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

#include "yaul_lexer.h"

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
	PT_CHUNK,					//
	PT_BLOCK,
	PT_STATEMENT,
	PT_LASTSTATEMENT,
	PT_VAR_ASSIGN,		//
	PT_EXP,						//
	PT_FUNCTIONCALL,	
	PT_FUNCTION,
	PT_EXPLIST,
	PT_WHILE,					//
	PT_IF,						//
	PT_FUNCNAME,
	PT_FUNCBODY,
	PT_OPERATOR,			//
	PT_VAR_DECLARE,		//
	PT_CONST,					//
	PT_VAR,						//
	
	PT_QTY
};

struct ParseNode;

struct OperatorPrecendence {
	OperatorPrecendence(int precendence_, bool unary_) : precendence(precendence_), left_assoc(true), unary(unary_), valid(true) {}
	
	int					precendence;
	bool				left_assoc;
	bool				unary;
	bool				valid;
};

struct ParseNode {
	ParseTokenType					type;
	LexToken							 *lextoken;
	int											v;
	std::vector<ParseNode*>	children;
};

class ParseException {
	std::string _error;
	
public:
	ParseException(const char* error) { _error = error; }
	const char* error() { return _error.c_str(); }
};

class Parser {
	LexToken		*_tokens;
	LexToken		*_curtk;
	
	ParseNode		*_topNode;
	ParseNode		*_curNode;
	
	static OperatorPrecendence *_opTable;
	static void initOperatorTable();
	
private: 
	// error handling
	void throwError(const char* err, ...);
		
private:	
	// creates a node with given type
	static	ParseNode*	createNode(ParseTokenType type);
	// adds a node as a child to the current node and sets current node to child
	static	ParseNode*	addNode(ParseNode *node, ParseNode *parentNode);
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
	
	// prints debug info
	static void	printNodeInfo(FILE *file, ParseNode *node, int indentation = 0);
	
	// deallocs node children
	static void	deallocChildren(ParseNode *node);
	
private:
	// chunk ::= block
	ParseNode*  parseChunk();
	
	// block ::= {statement ';'} [laststatement ';']
	ParseNode*  parseBlock();
	ParseNode*  parseStatement();
	ParseNode*  parseVarAssignment();
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
	void reset();
	
	bool parse(ParseNode **topNode);
	
	void saveDebugTree(FILE* file);
};

#endif