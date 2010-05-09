/*
 *  Lexer.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/29/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#define TOKEN_BUFF_STEP 255;

#include "yaul_lexer.h"

const char* Lexer::token_strings[] = {
	"TK_NONE",
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
	
	"TK_IF",
	"TK_FUNCTION",
	"TK_LOCAL",
	"TK_WHILE",
	"TK_DO",
	"TK_THEN",
	"TK_END",
	"TK_ELSE",
	"TK_VAR",
	"TK_RETURN",
	"TK_BREAK",
	
	"TK_LESS",
	"TK_GREATER",
	"TK_LEQ",
	"TK_GREQ",
	
	
	"TK_EQUAL",
	"TK_NEQ",
	"TK_NOT",
	
	"TK_BIN_AND",
	"TK_BIN_OR",
	"TK_BIN_INV",
	
	"TK_AND",
	"TK_OR",
	"TK_BIN_XOR",
	
	"TK_MOD",
	
	"TK_EOF",
	"TK_UNKNOWN",
	"TK_QTY"
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
	_curline = 1;
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
	
	_curline = 1;
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
			tk->intval = atoi(tk->data);
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
	
	LexToken *tokens_flat = (LexToken*)calloc(tk_count+1, sizeof(LexToken));
	memset(tokens_flat, 0, (tk_count+1)*sizeof(LexToken));
	for (int i=0; i<tk_count; ++i )  {
		memcpy(&tokens_flat[i], tokens_s[i], sizeof(LexToken));
	}
	
	*tokens_ = tokens_flat;
	
	for (int i=0; i<tk_count; ++i ) {
		printf("%s '%s'\n", token_strings[tokens_flat[i].type], tokens_flat[i].data);
		tokens_s++;
	}
	
	while (*tokens_s) {
		free(*tokens_s++);
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
	token->dblval = atof(token->data);
	return token;
}

LexToken* Lexer::getToken() {
	LexToken *token = (LexToken*)calloc(1, sizeof(LexToken));
	memset(token, 0, sizeof(LexToken));
	char		 *token_data = token->data;
	
	token->line = _curline;
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
				case '=': 
					token->type = TK_ASSIGN;   
					if ( *_buff == '=' ) {
						token->type = TK_EQUAL;
						_buff++;
					}
					return token;
				
				case '!': 
					token->type = TK_NOT;  
					if ( *_buff == '=' ) {
						token->type = TK_NEQ;
						_buff++;
					}
					return token;
					
				case '>': token->type = TK_LESS;  return token;
				case '<': token->type = TK_GREATER; return token;
				case '&': token->type = TK_BIN_AND;  return token;
				case '|': token->type = TK_BIN_OR; return token;
				case '~': token->type = TK_BIN_INV; return token;
				case '%': token->type = TK_MOD; return token;
					
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
							if ( isalnum(ch) || ch == '_') {
								*token_data++ = ch;
							}
							else break;
						}
						TLexToken tktype = getReservedWord( token->data );
						if ( tktype != TK_QTY ) {
							token->type = tktype;
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

TLexToken Lexer::getReservedWord(const char *word) {
	struct _ResWord {
		const char* word;
		TLexToken type;
	};
	
	static _ResWord words[] = {
		{"if", TK_IF},
		{"function", TK_FUNCTION},
		{"local", TK_LOCAL},
		{"while", TK_WHILE},
		{"do", TK_DO},
		{"then", TK_THEN},
		{"end", TK_END},
		{"else", TK_ELSE},
		{"var", TK_VAR},
		{"return", TK_RETURN},
		{"break", TK_BREAK},
		{"and", TK_AND},
		{"or", TK_OR},
		{"xor", TK_BIN_XOR},
		
	};
	
	for ( int i=0; i < sizeof(words)/sizeof(_ResWord); i++ ) {
		if (!strcmp(word, words[i].word) ) {
			return words[i].type;
		}
	}
	
	return TK_QTY;
}

const char* Lexer::getTokenTypeString(TLexToken type) {
	assert( type < TK_QTY );
	return token_strings[type];
}
