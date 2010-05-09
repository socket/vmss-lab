/*
 *  Parser.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 4/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "yaul_parser.h"

OperatorPrecendence *Parser::_opTable = NULL;

static const char *parseTokenTypeStr[] = {
	"PT_CHUNK",
	"PT_BLOCK",
	"PT_STATEMENT",
	"PT_LASTSTATEMENT",
	"PT_VAR_ASSIGN",
	"PT_EXP",
	"PT_FUNCTIONCALL",
	"PT_FUNCTION",
	"PT_EXPLIST",
	"PT_WHILE",
	"PT_IF",
	"PT_FUNCNAME",
	"PT_FUNCBODY",
	"PT_OPERATOR",
	"PT_VAR_DECLARE",
	"PT_CONST",
	"PT_VAR",
	
	"PT_QTY"
};


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

void Parser::reset() {
	_curtk = _tokens;
	deallocChildren(_topNode);
	delete _topNode;
	_topNode = NULL;
}

void Parser::throwError(const char* err, ...) {
	char buff[256];
	char buff2[256];
	memset(buff, 0, sizeof(buff));
	memset(buff2, 0, sizeof(buff2));
	
	va_list args;
	va_start(args, err);
	vsnprintf( buff, sizeof(buff) - 1, err, args); 
	sprintf(buff2, "syntax error: %s", buff);

	throw ParseException(buff2);
}

void Parser::initOperatorTable() {
	_opTable = (OperatorPrecendence*)calloc(TK_QTY, sizeof(OperatorPrecendence));
	assert( _opTable );
	memset(_opTable, 0, sizeof(OperatorPrecendence)*TK_QTY);
	
	_opTable[TK_PLUS] = OperatorPrecendence(3, false);
	_opTable[TK_MINUS] = OperatorPrecendence(3, false);
	_opTable[TK_MUL] = OperatorPrecendence(2, false);
	_opTable[TK_DIV] = OperatorPrecendence(2, false);
	
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
		throwError("(%s:%d) unexpected token %s (was looking for %s) ", "chunk", _curtk->line, Lexer::getTokenTypeString(_curtk->type), 
							 Lexer::getTokenTypeString(type));
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

bool Parser::parse() {
	bool result = true;
	try {
		parseChunk();
	}
	catch (ParseException e) {
		result = false;
		printf( "%s\n", e.error() );
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////
ParseNode* Parser::parseChunk() {
	_topNode = createNode(PT_CHUNK);
	assert( _topNode );
	
	addNode( parseBlock(), _topNode);
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

ParseNode* Parser::parseVarAssignment() {
	if ( acceptToken(TK_IDENT) ) {
		ParseNode *node = createNode(PT_VAR_ASSIGN);
		node->lextoken = _curtk-1;
		expectToken(TK_ASSIGN);
		expectNode(PT_EXP, parseExp(), node);
		
		return node;
	}
	return NULL;
}

ParseNode* Parser::parseVar() {
	if ( acceptToken(TK_IDENT) ) {
		ParseNode *node = createNode(PT_VAR);
		node->lextoken = _curtk-1;
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
	bool done = false;
	
	ParseNode *node = createNode(PT_EXP);
	std::stack<LexToken*> op_stack;
	
	LexToken *tk_s = _curtk;
	
	while (_curtk && !done) {
		switch (_curtk->type) {
			case TK_IDENT:
				if ( (_curtk+1)->type == TK_LPAR ) { // function call 
					expectNode(PT_FUNCTIONCALL, parseFunctionCall(), node);
				} 
				else {	// variable
					expectNode(PT_VAR, parseVar(), node );
					_curtk--; // we will call nextToken later, so compensate this
				}
				
			case TK_INT:
			case TK_FLOAT:
				if ( _curtk->type != TK_IDENT ) {
					ParseNode *n = createNode(PT_CONST);
					n->lextoken = _curtk;
					addNode(n, node);
				}
				while (op_stack.size() > 0) {
					LexToken *stack_top = op_stack.top();
					if ( _opTable[stack_top->type].valid && _opTable[stack_top->type].unary ) {
						ParseNode *n = createNode(PT_OPERATOR);
						n->lextoken = stack_top;
						n->v = stack_top->type;
						addNode(n, node);
						
						op_stack.pop();
					}
					else {
						break;
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
						if ( 1 == op_stack.size() ) {
							//throwError("mismatched parenthesis");
							nextToken();
							return node;
							
						}
						ParseNode *n = createNode(PT_OPERATOR);
						n->lextoken = stack_top;
						n->v = stack_top->type;
						addNode(n, node);
						
						op_stack.pop();
					}
					else {
						op_stack.pop();
						break;
					}

				}
				break;
				
			default:
			if ( _opTable[_curtk->type].valid ) {
				if ( !_opTable[_curtk->type].unary ) {
					while (op_stack.size() > 0) {
						LexToken *stack_top = op_stack.top();
						assert( stack_top );
						if ( _opTable[stack_top->type].valid && ( 
								( _opTable[_curtk->type].left_assoc && _opTable[_curtk->type].precendence > _opTable[stack_top->type].precendence ) ||
								(! _opTable[_curtk->type].left_assoc && _opTable[_curtk->type].precendence >= _opTable[stack_top->type].precendence ) )
								) {
							ParseNode *n = createNode(PT_OPERATOR);
							n->lextoken = stack_top;
							n->v = stack_top->type;
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
				done = true;
			}
			break;
		}
		if ( ! done ) {
			nextToken();
		}
	}
	
	while (op_stack.size() > 0) {
		LexToken *stack_top = op_stack.top();
		if ( stack_top->type == TK_LPAR || stack_top->type == TK_RPAR ) {
			throwError("Mismatched parenthesis in expression");
			return NULL;
		}
		
		ParseNode *n = createNode(PT_OPERATOR);
		n->lextoken = stack_top;
		n->v = stack_top->type;
		addNode(n, node);
		
		op_stack.pop();		
	}
	
	return node;
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
	
	if ( acceptNode( PT_VAR_ASSIGN, parseVarAssignment(), node)) { }
	else if ( acceptNode( PT_FUNCTIONCALL, parseFunctionCall(), node ) ) { }
	else if ( acceptToken(TK_VAR) ) {
		ParseNode *w = addNode(createNode(PT_VAR_DECLARE), node);
		expectToken(TK_IDENT);
		w->lextoken = _curtk-1;
	}
	else if ( acceptToken(TK_WHILE) ) {
		ParseNode *w = addNode(createNode(PT_WHILE), node);
		expectNode(PT_EXP, parseExp(), w);
		expectToken(TK_DO);
		expectNode(PT_BLOCK, parseBlock(), w);
		expectToken(TK_END);
	}
	else if ( acceptToken(TK_IF) ) {
		ParseNode *w = addNode(createNode(PT_IF), node);
		expectNode(PT_EXP, parseExp(), w);
		expectToken(TK_THEN);
		expectNode(PT_BLOCK, parseBlock(), w);
		if ( acceptToken(TK_ELSE) ) {
			expectNode(PT_BLOCK, parseBlock(), w);			
		}
		expectToken(TK_END);
	}
	else if ( acceptToken(TK_FUNCTION) ) {
		ParseNode *w = addNode(createNode(PT_FUNCTION), node);
		expectNode(PT_FUNCNAME, parseFuncName(), w);
		expectNode(PT_FUNCBODY, parseFuncBody(), w);
	}
	else {
		free( node );
		node = NULL;
	}
	
	return node;
}

////////////////////////////////////////////////////////////////////////////////////////////
void	Parser::printNodeInfo(FILE *file, ParseNode *node, int indentation) {
	assert( file );
	assert( node );
	for (int i=0; i<indentation; ++i) {
		fprintf(file, "\t");
	}
	fprintf(file, "%s", parseTokenTypeStr[node->type]);
	if ( node->lextoken ) {
		fprintf(file, " <- %s('%s')", Lexer::getTokenTypeString(node->lextoken->type), node->lextoken->data);	
	}
	fprintf(file, "\n");
	for (int i=0; i<node->children.size(); ++i ) {
		printNodeInfo(file, node->children[i], indentation+1);
	}
}

void Parser::saveDebugTree(FILE* file) {
	assert( file );
	printNodeInfo(file, _topNode);
}

void Parser::deallocChildren(ParseNode *node) {
	assert( node );
	for (int i=0; i<node->children.size(); ++i ) {
		deallocChildren( node->children[i] );
		delete node->children[i];
	}	
	node->children.clear();
}
