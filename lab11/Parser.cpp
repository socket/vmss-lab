/*
 *  Parser.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "Parser.h"

Parser::Parser() {
	_tokens = NULL;
	_curtk = NULL;
}

void Parser::init(LexToken *tokens) {
	assert( tokens );
	_tokens = tokens;
	_curtk = tokens;
}



///////////////////////////////////////////////////////////////////////////////////////////

// creates a node with given type
ParseNode* Parser::createNode(ParseTokenType type)  {
	ParseNode* node = new ParseNode();
	assert( node );
	node->type = type;
	
	return node;
}

// adds a node as a child to the current node and sets current node to child
void Parser::addNode(ParseNode *node, ParseNode *parentNode) {
	assert( node );
	assert( parentNode );
	assert( node != parentNode );
	
	parentNode->children.push_back( node );
}

// accepts given node and adds it to parent node
bool Parser::acceptNode(ParseTokenType type, ParseNode *node, ParseNode *parentNode) {
	assert( node );
	if ( node->type == type ) {
		assert( parentNode );
		addNode( node, parentNode );
		return true;
	}
	return false;
}

// same as accept but throws an error if node does not match type
bool Parser::expectNode(ParseTokenType type, ParseNode *node, ParseNode *parentNode) {
	if ( !acceptNode(type, node, parentNode) ) {
		throwError("invalid node");
		return false;
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

// accepts optional token from input stream
bool Parser::acceptToken(TLexToken type) {
	assert( type < TK_QTY );
	assert( _curtk );
	assert( _curtk->type < TK_QTY );
	
	if ( _curtk->type == type ) {
		nextToken();
		return true;
	}
	return false;
}

// same, but token is required
bool Parser::expectToken(TLexToken type) {	
	if ( !acceptToken(type) ) {
		throwError("invalid token");
		return false;
	}
	return true;
}

// moves to next token
void Parser::nextToken() {
	assert( _curtk );
	_curtk++;
	assert( _curtk );
}

///////////////////////////////////////////////////////////////////////////////////////////
ParseNode* Parser::parseChunk() {
	_topNode = createNode(PT_CHUNK);
	assert( _topNode );
	
	parseBlock( _topNode );
	return _topNode;
}



///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
/*
void Parser::nextToken() {
	assert(_curtk != NULL);
	_curtk++;
}

bool Parser::expect(TLexToken tk) {
	if ( accept(tk) )
		return true;
	
	return false;
}

bool Parser::expect_res(bool res, const char* name) {
	if ( accept(tk) )
		return true;
	
	return false;
}

bool Parser::th_expect(TLexToken tk) {
	if ( accept(tk) )
		return true;
	
	return false;
}

bool Parser::accept(TLexToken tk) {
	if ( tk == _curtk->type ) {
		nextToken();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////
bool Parser::doChunk() {
	doBlock();
}

bool Parser::doBlock() {
	while( doStatement() ) {
		expect(TK_SEMICOLON);
	}
	if ( 	doLastStatement() ) {
		expect(TK_SEMICOLON);
	}
	return true;
}

bool Parser::doStatement() {
	if ( doVar() ) {
		expect(TK_ASSIGN);
		expect_res( doExp(), "EXPR" );
	}	
	else if ( doFunctionCall() ) {

	}
	else if ( accept(TK_WHILE) ) {
		expect_res( doExp(), "WHILE_EXPR" );
		expect(TK_DO);
		expect_res( doBlock(), "BLOCK" );
		expect(TK_END);
	}
	else if ( accept(TK_IF ) ) {
		expect_res( doExp(), "IF_EXPR" );
		expect(TK_THEN);
		expect_res( doBlock(), "IF_BLOCK");
		if ( accept(TK_ELSE) ) {
			expect_res( doBlock(), "ELSE_BLOCK");			
		}
		expect(TK_END);
	}
	else if ( accept(TK_FUNCTION) ) {
		expect(TK_LITERAL);
		expect_res(doFuncbody(), "FUNC_BODY");
	}
}
	
bool Parser::doVar() {
	return expect(TK_LITERAL);
}

	
void doLastStatement();
void doNamelist();
void doExplist();
void doPrefixexp();
void doFunctionCall();
void doArgs();
void doFunction();
void doFuncbody();
void doBinop();
void doUnop();
*/