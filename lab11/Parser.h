/*
 *  Parser.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _parser_h
#define _parser_h

#include <assert.h>
#include <stack>
#include <vector>
#include <string>

#include "Lexer.h"

/* EBNF syntax form
 chunk ::= block
 block ::= {statement ';'} [laststatement ';']
 statement ::= 'var' var '=' exp | 
							functioncall |
							`while` exp `do` block `end` |
							`if` exp then `block` [else block] end |
							`function` funcname funcbody |
							`local` var ['=' exp]

 laststatement ::= `return` exp | break
 funcname ::= Name
 var ::= Name
 namelist ::= Name {',' Name}
 explist  ::= {exp ','} exp
 exp ::= nil | false | true | Number | String | exp binop exp | unop exp
 prefixexp ::= var | functioncall | '(' exp ')'
 functioncall ::= prefixexp args
 args ::= '(' [explist] ')'
 function ::= 'function' funcbody
 funcbody ::= '(' [namelist] ')' block end
 binop ::= '+' | '-' | '*' | '/' | '^' | '%' | '..' | '<' |
					 '<' | '<=' | '>' | '>=' | '==' | 'and' | 'or'
 unop ::= '-' | '!'
*/
enum ParseTokenType {
	PT_CHUNK,
	PT_BLOCK,
	PT_STATEMENT,
	PT_LASTSTATEMENT,
	PT_VAR,
	PT_EXP,
	PT_FUNCTIONCALL,
	PT_FUNCTION,
	PT_EXPLIST,
	PT_WHILE,
	PT_IF,
	PT_FUNCNAME,
	PT_FUNCBODY,
	
	PT_QTY
};

struct ParseNode;
struct ParseNode {
	ParseTokenType					type;
	std::vector<ParseNode*>	children;
};

class ParseException {
	std::string _error;
	
public:
	ParseException(const char* _error);
};

class Parser {
	LexToken		*_tokens;
	LexToken		*_curtk;
	
	ParseNode		*_topNode;
	ParseNode		*_curNode;
	
private: 
	// error handling
	void throwError(const char* err, ...);
	
private:
	// returns current node
	ParseNode*	getNode();
	
	// creates a node with given type
	ParseNode*	createNode(ParseTokenType type);
	// adds a node as a child to the current node and sets current node to child
	ParseNode*	addNode(ParseNode *node, ParseNode *parentNode);
	// accepts given node and adds it to parent node
	bool				acceptNode(ParseTokenType type, ParseNode *node, ParseNode *parentNode);
	// same as accept but throws an error if node does not match type
	bool				expectNode(ParseTokenType type, ParseNode *node, ParseNode *parentNode);
	
	// accepts optional token from input stream
	bool				acceptToken(TLexToken type);
	// same, but token is required
	bool				expectToken(TLexToken type);
	// moves to next token
	void				nextToken();
	
public:
	// chunk ::= block
	ParseNode*  parseChunk();
	
	// block ::= {statement ';'} [laststatement ';']
	ParseNode*  parseBlock();
	ParseNode*  parseStatement();
	ParseNode*  parseVar();
	
	ParseNode*  parseFunctionCall();

	ParseNode*  parseExp();
	ParseNode*  parseExpList();
	ParseNode*  parseLastStatement();
	ParseNode*  parseFuncName();
	ParseNode*  parseFuncBody();
	
public:
	Parser();
	
	void init(LexToken *tokens);
	
	bool parse();
};

#endif