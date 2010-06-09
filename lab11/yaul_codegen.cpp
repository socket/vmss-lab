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

#include "yaul_opcodes_str.h"

CodeGen::CodeGen() {
	
}

CodeGen::~CodeGen() {
	
}


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

bool CodeGen::generate(ParseNode *node, yaul_op **code, int *count) {
	_code.clear();
	genChunk(node, NULL);

	*code = (yaul_op*)malloc( sizeof(yaul_op) * (_code.size() + 1) );
	memset(*code, 0, sizeof(yaul_op) * (_code.size() + 1) );
	for ( int i=0; i<_code.size(); ++i ) {
		*(*code + i) = *_code[i];
	}
	if ( count ) {
		*count = _code.size();
	}
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
		ParseNode *cnode = node->children[i];
		switch (cnode->type) {
				
		case PT_BLOCK:
			genBlock(cnode, node);
			break;
			
		default:
			throwError("Unknown node in chunk");
			break;
		}
	}
	addOp(OP_RETN);
}

void CodeGen::genBlock(ParseNode *node, ParseNode *parent) {
	_blocks.push( CodeBlock() );
	for( int i=0; i < node->children.size(); ++i ) {
		ParseNode *cnode = node->children[i];
		switch (cnode->type) {
			case PT_STATEMENT:
				genStatement(cnode, node);
				break;
			
			case PT_LASTSTATEMENT:
				addOp(OP_RETN);
				break;
				
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
			addOp(OP_PUSH_STRING, (long)c_node->lextoken->data);
			genExp(c_node, node);
			if ( getCodeBlock()->hasLocal(c_node->lextoken->data) ) {
				//addOp(OP_SETLOCAL); // not impl
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
				jne->_operand = calcJumpPos(jne);
				genBlock(c_node->children[2], c_node);
				jmp->_operand = calcJumpPos(jmp);
			}
			else {
				yaul_op *jne = addOp(OP_JNE);
				genBlock(c_node->children[1], c_node);
				jne->_operand = calcJumpPos(jne);				
			}

			break;
		
		case PT_WHILE:
		{
			yaul_op *nop = addOp(OP_NOP);
			genExp(c_node->children[0], c_node);
			addOp(OP_CMP, 1);
		
			yaul_op *jne = addOp(OP_JNE);
			genBlock( c_node->children[1], c_node );
			addOp(OP_JMP, -calcJumpPos(nop)-1 );
			jne->_operand = calcJumpPos(jne);
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
			
		case PT_FUNCTIONCALL:
			genFuncCall( c_node, node );
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
				case TK_MOD: addOp(OP_MOD); break;
				case TK_NOT: addOp(OP_NOT); break;
				case TK_LEQ: addOp(OP_LEQ); break;
				case TK_GREQ: addOp(OP_GEQ); break;
				case TK_EQUAL: addOp(OP_EQ); break;
				case TK_NEQ: addOp(OP_EQ); addOp(OP_NOT); break;
				case TK_LESS: addOp(OP_LESSER); break;
				case TK_GREATER: addOp(OP_GREATER); break;
				case TK_AND: addOp(OP_AND); break;
				case TK_OR: addOp(OP_OR); break;
				case TK_BIN_XOR: addOp(OP_XOR); break;
				case TK_BIN_AND: addOp(OP_BIN_AND); break;
				case TK_BIN_OR: addOp(OP_BIN_OR); break;
				case TK_BIN_INV: addOp(OP_BIN_INV); break;
				case TK_IMPL: addOp(OP_IMPL); break;
				case TK_MODP: addOp(OP_MODP); break;
				case TK_SHF: addOp(OP_SHF); break;
				default:
					throwError("Unknown operator '%s'", Lexer::getTokenTypeString( cnode->lextoken->type ));
					break;
			}
		}
		else if ( cnode->type == PT_CONST ) {
			if ( cnode->lextoken->type == TK_INT ) {
				addOp(OP_PUSH_INT, cnode->lextoken->intval);
			}
			else if ( cnode->lextoken->type == TK_FLOAT ) {
				addOp(OP_PUSH_DOUBLE, (long)*((long*)(&cnode->lextoken->dblval)));
			}
			else if ( cnode->lextoken->type == TK_LITERAL ) {
				addOp(OP_PUSH_STRING, (long)cnode->lextoken->data);
			}
			getCodeBlock()->_push_count++;
		}
		else if ( cnode->type == PT_VAR ) {
			addOp(OP_PUSH_STRING, (long)cnode->lextoken->data);
			if ( getCodeBlock()->hasLocal(cnode->lextoken->data) ) {
				assert(0);//not implemented
				//addOp(OP_GETLOCAL);
			}
			else {
				addOp(OP_GETGLOBAL);
			}
		}
		else if ( cnode->type == PT_EXP ) {
			genExp(cnode, node);
		}
		else if ( cnode->type == PT_FUNCTIONCALL ) {
			genFuncCall(cnode, node);
		}
		else {
			throwError("Unsupported node in expression");
		}

	}
}

void CodeGen::genFuncCall(ParseNode *node, ParseNode *parent) {
	ParseNode* expList = node->children[0];
	if ( expList->type == PT_EXPLIST ) {
		for( int i=0; i < node->children.size(); ++i ) {
			ParseNode *cnode = node->children[i];
			genExp(cnode, node);
		}
		addOp(OP_PUSH_STRING, (long)node->lextoken->data);
		addOp(OP_GETGLOBAL);
		addOp(OP_CALLFUNC);
	}
	else {
		throwError("invalid node in explist");
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// service functions

yaul_op* CodeGen::addOp(yaul_opcode opcode, long arg) {
	yaul_op *op = new yaul_op();
	op->_opcode = opcode;
	op->_operand = arg;
	_code.push_back(op);
	
	return op;
}


int CodeGen::calcJumpPos(yaul_op* op) {
	int pos = 0;
	for (int i=_code.size()-1; i >= 0; --i ) {
		if ( _code[i] == op ) {
			return pos;
		}
		pos++;
	}
	throwError("internal error");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// debug

void CodeGen::print(FILE *f) {
	for (int i=0; i < _code.size(); ++i ) {
		yaul_op &op = *_code[i];
		fprintf(f, "%04d: %s, ", i, opcodes_str[op._opcode] );

		if ( op._opcode == OP_PUSH_DOUBLE ) {
			fprintf(f, "%f",  *((double*)(&op._operand)) );
		} 
		else if ( op._opcode == OP_PUSH_STRING ) {
			fprintf(f, "'%s'", (const char*)op._operand);
		}
		else {
			fprintf(f, "%d", (signed int)op._operand);
		}
		fprintf(f, "\n");
	}	
}
