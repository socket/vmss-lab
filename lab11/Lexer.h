/*
 *  Lexer.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/29/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _Lexer_h
#define _Lexer_h

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

enum TLexToken {
	TK_NONE,
	TK_INT,
	TK_FLOAT,
	TK_LITERAL,
	TK_IDENT,
	
	// parenthesis
	TK_LPAR,
	TK_RPAR,
	
	//
	TK_DOT,
	TK_COLON,
	TK_SEMICOLON,
	TK_COMMA,
	
	// operators
	TK_PLUS,
	TK_MINUS,
	TK_MUL,
	TK_DIV,
	
	//
	TK_ASSIGN,
	
	TK_IF,
	TK_FUNCTION,
	TK_LOCAL,
	TK_WHILE,
	TK_DO,
	TK_THEN,
	TK_END,
	TK_ELSE,
	TK_VAR,
	TK_RETURN,
	TK_BREAK,
	
	TK_LESS,
	TK_GREATER,
	TK_LEQ,
	TK_GREQ,
	
	TK_EQUAL,
	TK_NEQ,
	TK_NOT,
	
	TK_EOF,
	TK_UNKNOWN,
	TK_QTY
};

struct LexToken {
	TLexToken type;
	char data[256];
	
	void*			pv;
	int				intval;
	double		dblval;
	
	int	_line;
};

class Lexer {
	char  *_buff_start;
	char	*_buff;
	
	int		 _buffSize;
	int		 _curline;
	int		 _errline;

	static const char *token_strings[];

public:
	Lexer();
	virtual ~Lexer();
	
	int analyze(const char* input, LexToken **tokens, int *count);
	
	static const char* getTokenTypeString(TLexToken type);
	
protected:
	LexToken* getToken();
	char			nextChar();	
	LexToken* makeDoubleNumberToken(LexToken *tkLeft, LexToken *tkRight);
	TLexToken getReservedWord(const char *word);

};

#endif //_Lexer_h