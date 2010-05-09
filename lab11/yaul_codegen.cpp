/*
 *  yaul_codegen.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#include "yaul_lexer.h"
#include "yaul_parser.h"
#include "yaul_codegen.h"


void CodeGen::throwError(const char* err, ...) {
	char buff[256];
	char buff2[256];
	memset(buff, 0, sizeof(buff));
	memset(buff2, 0, sizeof(buff2));
	
	va_list args;
	va_start(args, err);
	vsnprintf( buff, sizeof(buff) - 1, err, args); 
	sprintf(buff2, "syntax error: %s", buff);
	
	throw CodeGenException(buff2);
}

bool CodeGen::generate(ParseNode *node, yaul_op **code) {
	_code.clear();
	genChunk(node, NULL);

	return true;
}

CodeBlock* CodeGen::getCodeBlock() {
	if ( _blocks.size() > 0 ) {
		return &_blocks.top();
	}
	return NULL;
}


void CodeGen::genChunk(ParseNode *node, ParseNode *parent) {
	for( int i=0; i < node->children.size(); ++i ) {
		switch (node->type) {
				
		case PT_BLOCK:
			genBlock(node, parent);
			return;
			
		default:
			throwError("Unknown node in chunk");
			break;
		}
	}
}

void CodeGen::genBlock(ParseNode *node, ParseNode *parent) {
	_blocks.push( CodeBlock() );
	for( int i=0; i < node->children.size(); ++i ) {
		switch (node->type) {
			case PT_STATEMENT:
				genStatement(node, parent);
				return;
			
			case PT_LASTSTATEMENT:
				addOp(OP_RETN);
				return;
				
			default:
				throwError("Unknown statement in block");
				break;
		}
	}	
	
	_blocks.pop();
}


void CodeGen::genStatement(ParseNode *node, ParseNode *parent) {
	assert( node->children.size() > 0 );
	ParseNode *c_node = node->children[0];
	switch (c_node->type) {
		case PT_VAR_ASSIGN:
			addOp(OP_PUSH_STRING, c_node->lextoken->data);
			genExp(c_node, node);
			if ( getCodeBlock()->hasLocal(c_node->lextoken->data) ) {
				addOp(OP_SETLOCAL);
			}
			else {
				addOp(OP_SETGLOBAL);
			}			
			break;
		
		case PT_IF:
			// 00 <EXP>
			// 01 CMP, 1
			// 02 JNE, 05
			// 03 <BLOCK> 
			// 04 JMP, 06
			// 05 <BLOCK>
			// 06 ...
			
			genExp(c_node->children[0], c_node);
			addOp(OP_CMP, 1);
			if ( c_node->children.size() == 3 )  { // if..else
				yaul_op *jne = addOp(OP_JNE);
				genBlock(c_node->children[1], c_node);
				yaul_op *jmp = addOp(OP_JMP);
				jne->_operand = (void*)calcJumpPos(jne);
				genBlock(c_node->children[2], c_node);
				jmp->_operand = (void*)calcJumpPos(jmp);
			}
			else {
				yaul_op *jne = addOp(OP_JNE);
				genBlock(c_node->children[1], c_node);
				jne->_operand = (void*)calcJumpPos(jne);				
			}

			break;
		
		case PT_WHILE:
		{
			yaul_op *nop = addOp(OP_NOP);
			genExp(c_node->children[0], c_node);
			addOp(OP_CMP, 1);
		
			yaul_op *jne = addOp(OP_JNE);
			genBlock( c_node->children[1], c_node );
			addOp(OP_JMP, (void*)calcJumpPos(nop));
			jne->_operand = (void*)calcJumpPos(jne);
			}
			break;
		
		case PT_VAR_DECLARE:
			if ( !getCodeBlock()->hasLocal(c_node->lextoken->data) ) {
				getCodeBlock()->addLocal( c_node->lextoken->data );
			}
			else {
				
				throwError("redeclaration of local var '%s'", c_node->lextoken->data);
			}
			break;
			
		default:
			throwError("unknown statement");
			break;
	}
}

void CodeGen::genExp(ParseNode *node, ParseNode *parent) {
	// expressions are presented as nodes in back-notation
	for( int i=0; i < node->children.size(); ++i ) {
		ParseNode *cnode = node->children[i];
		assert( cnode );
		
		if ( cnode->type == PT_OPERATOR ) {
			switch (cnode->lextoken->type) {
				case TK_PLUS: addOp(OP_ADD); break;
				case TK_MINUS: addOp(OP_SUB); break;
				case TK_MUL: addOp(OP_MUL); break;
				case TK_DIV: addOp(OP_DIV); break;
				case TK_NOT: addOp(OP_NOT); break;
				case TK_LEQ: addOp(OP_LEQ); break;
				case TK_GREQ: addOp(OP_GEQ); break;
				case TK_EQUAL: addOp(OP_EQ); break;
				case TK_NEQ: addOp(OP_EQ); addOp(OP_NOT); break;
				case TK_LESS: addOp(OP_LESSER); break;
				case TK_GREATER: addOp(OP_GREATER); break;
				
				default:
					throwError("Unknown operator '%s'", Lexer::getTokenTypeString( cnode->lextoken->type ));
					break;
			}
		}
		else if ( cnode->type == PT_CONST ) {
			if ( cnode->lextoken->type == TK_INT ) {
				addOp(OP_PUSH_INT, (void*)cnode->lextoken->intval);
			}
			else if ( cnode->lextoken->type == TK_FLOAT ) {
				addOp(OP_PUSH_DOUBLE, (void*)*((int*)(&cnode->lextoken->dblval)));
			}
			getCodeBlock()->_push_count++;
		}
		else if ( cnode->type == PT_VAR ) {
			addOp(OP_PUSH_STRING, cnode->lextoken->data);
			if ( getCodeBlock()->hasLocal(cnode->lextoken->data) ) {
				addOp(OP_GETLOCAL);
			}
			else {
				addOp(OP_GETGLOBAL);
			}
		}
		else {
			throwError("Unsupported node in expression");
		}

	}
}