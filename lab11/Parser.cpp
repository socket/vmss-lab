/*
 *  Parser.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "Parser.h"

OperatorPrecendence *Parser::_opTable = NULL;

Parser::Parser() {
	if ( !_opTable ) {
		initOperatorTable(); 
	}
	_tokens = NULL;
	_curtk = NULL;
}

void Parser::init(LexToken *tokens) {
	assert( tokens );
	_tokens = tokens;
	_curtk = tokens;
}

void Parser::throwError(const char* err, ...) {
	printf("%s", err);
}

void Parser::initOperatorTable() {
	_opTable = (OperatorPrecendence*)calloc(TK_QTY, sizeof(OperatorPrecendence));
	assert( _opTable );
	memset(_opTable, 0, sizeof(OperatorPrecendence)*TK_QTY);
	
	_opTable[TK_PLUS] = OperatorPrecendence(1, true);
	_opTable[TK_MINUS] = OperatorPrecendence(1, true);
	_opTable[TK_MUL] = OperatorPrecendence(1, true);
	_opTable[TK_DIV] = OperatorPrecendence(1, true);
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
ParseNode* Parser::addNode(ParseNode *node, ParseNode *parentNode) {
	assert( node );
	assert( parentNode );
	assert( node != parentNode );
	
	parentNode->children.push_back( node );

	return node;
}

// accepts given node and adds it to parent node
bool Parser::acceptNode(ParseTokenType type, ParseNode *node, ParseNode *parentNode) {
	if ( node && node->type == type ) {
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
	
	addNode( _topNode, parseBlock() );
	return _topNode;
}

ParseNode*  Parser::parseBlock() {
	ParseNode *node = createNode(PT_BLOCK);
	//block ::= {statement ';'} [laststatement ';']
	while ( acceptNode( PT_STATEMENT, parseStatement(), node ) ) {
		expectToken(TK_SEMICOLON);
	}
	if ( acceptNode( PT_LASTSTATEMENT, parseLastStatement(), node ) ) {
		expectToken(TK_SEMICOLON);
	}
	return node;
}

ParseNode* Parser::parseLastStatement() {
	if ( acceptToken(TK_RETURN) || acceptToken(TK_BREAK) ) {
		return createNode(PT_LASTSTATEMENT);
	}
	
	return NULL;
}

ParseNode* Parser::parseVar() {
	if ( acceptToken(TK_VAR) ) {
		ParseNode *node = createNode(PT_VAR);
		expectToken(TK_IDENT);
		node->lextoken = _curtk-1;
		expectToken(TK_ASSIGN);
		expectNode(PT_EXP, parseExp(), node);
		
		return node;
	}
	return NULL;
}

ParseNode* Parser::parseFunctionCall() {
	if ( acceptToken(TK_IDENT) ) {
		ParseNode *node = createNode(PT_FUNCTIONCALL);
		node->lextoken = _curtk-1;
		expectToken(TK_LPAR);
		expectNode( PT_EXPLIST, parseExpList(), node);
		expectToken(TK_RPAR);
		return node;
	}
	return NULL;
}

ParseNode* Parser::parseExpList() {
	ParseNode *node = createNode(PT_EXPLIST);
	while ( acceptNode(PT_EXP, parseExp(), node) ) {
		expectToken(TK_COMMA);		// ToDo!
	}
	return node;
}

ParseNode* Parser::parseExp() {
	bool unary = false;
	
	ParseNode *node = createNode(PT_EXP);
	std::stack<LexToken*> op_stack;
	
	LexToken *tk_s = _curtk;
	
	while (_curtk) {
		switch (_curtk->type) {
			case TK_IDENT:
				if ( (_curtk+1)->type == TK_LPAR ) { // function call 
					expectNode(PT_FUNCTIONCALL, parseFunctionCall(), node);
				} 
				else {	// variable
					expectNode(PT_VAR, parseVar(), node );
				}
				while (op_stack.size() > 0) {
					LexToken *stack_top = op_stack.top();
					if ( _opTable[stack_top->type].unary ) {
						ParseNode *n = createNode(PT_OPERATOR);
						n->lextoken = tk_s;
						n->v = tk_s->type;
						addNode(n, node);
						
						op_stack.pop();
					}
				}
				unary = false;
				break;
				
			case TK_LPAR:
				op_stack.push( _curtk );
				unary = true;
				break;
				
			case TK_RPAR:
				unary = false;
				while ( op_stack.size() > 0 ) {
					LexToken *stack_top = op_stack.top();
					if ( stack_top->type != TK_LPAR ) {
						if ( !op_stack.size() ) {
							//throwError("mismatched parenthesis");
							return node;
						}
						ParseNode *n = createNode(PT_OPERATOR);
						n->lextoken = tk_s;
						n->v = tk_s->type;
						addNode(n, node);
						
						op_stack.pop();
					}
				}
				break;
				
			default: {
				if ( _opTable[_curtk->type].valid ) {
					if ( !_opTable[_curtk->type].unary ) {
						while (op_stack.size() > 0) {
							LexToken *stack_top = op_stack.top();
							assert( stack_top );
							if ( ( _opTable[_curtk->type].left_assoc && _opTable[_curtk->type].precendence > _opTable[stack_top->type].precendence ) ||
									(! _opTable[_curtk->type].left_assoc && _opTable[_curtk->type].precendence >= _opTable[stack_top->type].precendence ) ) {
								ParseNode *n = createNode(PT_OPERATOR);
								n->lextoken = tk_s;
								n->v = tk_s->type;
								addNode(n, node);
								
								op_stack.pop();
							}
							else {
								break;
							}
						}
					}
					
					unary = true;
					op_stack.push( _curtk );
				}
				else {
					// unknown token (string ended)
					return node;
				}
			}
			break;
		}
		nextToken();
	}
	
	assert( 0 );
	
	free(node);
	return NULL;
}

ParseNode* Parser::parseFuncName() {
	throwError("Not implemented");	
	return NULL;
}

ParseNode* Parser::parseFuncBody() {
	throwError("Not implemented");
	return NULL;
}

/*
 statement ::= var '=' exp | 
 functioncall |
 `while` exp `do` block `end` |
 `if` exp then `block` [else block] end |
 `function` funcname funcbody |
 `local` var ['=' exp]
 */
ParseNode* Parser::parseStatement() {
	ParseNode *node = createNode(PT_STATEMENT);
	
	if ( acceptToken(TK_VAR)) {
		expectNode(PT_VAR, parseVar(), node);
	}
	else if ( acceptNode( PT_FUNCTIONCALL, parseFunctionCall(), node ) ) { }
	else if ( acceptToken(TK_WHILE) ) {
		ParseNode *w = addNode(node, createNode(PT_WHILE));
		expectNode(PT_EXP, parseExp(), w);
		expectToken(TK_DO);
		expectNode(PT_BLOCK, parseBlock(), w);
		expectToken(TK_END);
	}
	else if ( acceptToken(TK_IF) ) {
		ParseNode *w = addNode(node, createNode(PT_IF));
		expectNode(PT_EXP, parseExp(), w);
		expectToken(TK_THEN);
		expectNode(PT_BLOCK, parseBlock(), w);
		expectToken(TK_END);
	}
	else if ( acceptToken(TK_FUNCTION) ) {
		ParseNode *w = addNode(node, createNode(PT_FUNCTION));
		expectNode(PT_FUNCNAME, parseFuncName(), w);
		expectNode(PT_FUNCBODY, parseFuncBody(), w);
	}
	else {
		free( node );
		node = NULL;
	}
	
	return node;
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