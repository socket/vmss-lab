/*
 *  Lexer.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/29/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#define TOKEN_BUFF_STEP 255;

#include "Lexer.h"

const char* token_strings[] = {
		"TK_INT",
		"TK_FLOAT",
		"TK_LITERAL",
		"TK_IDENT",
		
		// parenthesis
		"TK_LPAR",
		"TK_RPAR",
		
		//
		"TK_DOT",
		"TK_COLON",
		"TK_SEMICOLON",
		"TK_COMMA",
		
		// operators
		"TK_PLUS",
		"TK_MINUS",
		"TK_MUL",
		"TK_DIV",
		
		//
		"TK_ASSIGN",
		
		"TK_EOF",
		"TK_UNKNOWN"
};

typedef enum {
	ls_def,
	ls_comment,
	ls_blockcomment,
} lex_state;

Lexer::Lexer() {
	_buff_start = 0;
	_buff =  0;
	
	_buffSize = 0;
	_curline = 0;
	_errline = 0;
}

Lexer::~Lexer() {
	if ( _buff ) {
		//free(_buff);
	}
}

int Lexer::analyze(const char* input, LexToken **tokens_, int *count) {
	int len = strlen( input );
	if ( len == 0 ) {
		return -1;
	}
	if ( !tokens_ || !count ) {
		return -2;
	}
	
	int tk_count = 0;
	int alloc_token_count = TOKEN_BUFF_STEP;
	LexToken **tokens = (LexToken**)malloc((alloc_token_count+1)*sizeof(LexToken*));
	LexToken **tokens_s = tokens;
	memset(tokens, 0, alloc_token_count+1);
	//*tokens = *tokens_;
	
	_curline = 0;
	_buff = (char*)malloc( len + 1 );
	strcpy(_buff, input);
	
	_buffSize = len+1;
	
	while ( LexToken *tk = getToken() ) {
		if ( ++tk_count > alloc_token_count ) {
			tokens = (LexToken**)realloc(tokens, (2*alloc_token_count+1)*sizeof(LexToken*));
			memset(tokens + alloc_token_count, 0, alloc_token_count+1);
			alloc_token_count *= 2;
		}
		
		LexToken *tkprev1 = (tk_count > 1) ? *(tokens-1) : 0;
		LexToken *tkprev2 = (tk_count > 2) ? *(tokens-2) : 0;
		
		// make doubles from number-dot-number
		if ( tk->type == TK_INT ) {
			tk->v.intval = atoi(tk->data);
			if ( tkprev1 && tkprev2 && tkprev1->type == TK_DOT && tkprev2->type == TK_INT ) {
				tokens--; *tokens-- = 0;		// pop 2 tokens from list
				tk_count -= 2;
				tk = makeDoubleNumberToken(tkprev2, tk);
			}
		}
		
		*tokens++ = tk;
		
		if ( tk->type == TK_EOF ) { // eof is eof
			break;
		}
	}
	*tokens = 0;
	*count = tk_count;
	*tokens = *tokens_s;
	
	while (tk_count--) {
		printf("%s '%s'\n", token_strings[(*tokens_s)->type], (*tokens_s)->data);
		tokens_s++;
	}
	
	return 0;
}

LexToken* Lexer::makeDoubleNumberToken(LexToken *tkLeft, LexToken *tkRight) {
	LexToken *token = (LexToken*)calloc(1, sizeof(LexToken));
	memset(token, 0, sizeof(LexToken));
	strcat(token->data, tkLeft->data);
	strcat(token->data, ".");
	strcat(token->data, tkRight->data);
	token->type = TK_FLOAT;
	token->v.dblval = atof(token->data);
	return token;
}

LexToken* Lexer::getToken() {
	LexToken *token = (LexToken*)calloc(1, sizeof(LexToken));
	memset(token, 0, sizeof(LexToken));
	char		 *token_data = token->data;
	
	token->type = TK_EOF;
	int state = ls_def;
	
	while( *_buff ) {
		char ch = *_buff++;
		if ( ch == '\n' ) { // strip newlines
			if ( state == ls_comment ) {
				state = ls_def;
			}
			
			_curline ++;
			continue;
		}
		if ( ch == ' ' || ch == '\r' || ch == '\t' ) {  // strip whitespaces
			continue;
		}
		
		if ( state == ls_def ) {
			*token_data++ = ch;
			switch (ch) {
				case '.': token->type = TK_DOT; return token;
				case ':': token->type = TK_COLON; return token;
				case ';': token->type = TK_SEMICOLON; return token;
				case ',': token->type = TK_COMMA; return token;
				case '+': token->type = TK_PLUS;  return token;
				case '-': token->type = TK_MINUS; return token;
				case '*': token->type = TK_MUL;   return token;
				case '=': token->type = TK_ASSIGN;   return token;
				case '/': 
				{
					char chn = *_buff++;
					if ( chn == '/') {
						// comment block begins until end of line
						*(--token_data) = 0;
						state = ls_comment;
						continue;
					}
					if ( chn == '*' ) {
						*(--token_data) = 0;
						state = ls_blockcomment;
						continue;
					}
					
					_buff--;
					token->type = TK_DIV;   
					return token;
				}

				case '(': token->type = TK_LPAR;  return token;
				case ')': token->type = TK_RPAR;  return token;
			
			  default:
				{
			// digits (numeric data)
					if ( isdigit(ch) ) {
						token->type = TK_INT;
						for(;;)  {
							ch = *_buff++;
							if ( isdigit(ch) ) {
								*token_data++ = ch;
							}
							else break;
						}
						_buff--;
						return token;
					}
					
					// vars
					if ( isalpha(ch) ) {
						token->type = TK_IDENT;
						for(;;)  {
							ch = *_buff++;
							if ( isalnum(ch) ) {
								*token_data++ = ch;
								break;
							}
							else break;
						}
						_buff--;
						return token;
					}
				}
			}
			
		}
		else if ( state == ls_blockcomment ) {
			if ( ch == '*' ) {
				ch = *_buff++;				
				if ( ch == '/' ) {
					state = ls_def;
					continue;
				}
				_buff--;
			}
		}
		
	}
	
	return token;
}