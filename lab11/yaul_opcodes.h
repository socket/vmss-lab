/*
 *  yaul_opcodes.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#ifndef _yaul_opcodes_h
#define _yaul_opcodes_h
typedef enum { 
	OP_NOT_USED,
	
	OP_PUSH_INT,
	OP_PUSH_DOUBLE,
	OP_PUSH_STRING,
	
	OP_POP,
	OP_RETN,
	
	OP_ADD,		// pop 2 push 1
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_BIN_INV,
	OP_BIN_AND,
	OP_BIN_OR,
	OP_NOT,
	OP_XOR,
	
	OP_EQ,
	OP_NEQ,
	OP_LEQ,
	OP_GEQ,
	OP_GREATER,
	OP_LESSER,
	
	OP_AND,
	OP_OR,
	
	OP_CMP,
	
	OP_JMP,
	
	OP_JNE,
	
	OP_CALLFUNC,
	
	OP_GETGLOBAL,
	OP_SETGLOBAL,
	
	OP_NOP,
	
	OP_4BYTE_PADDING = 0xFFFFFFFF
} yaul_opcode;

typedef struct  {
	yaul_opcode	_opcode;
	long				_operand;
} yaul_op_t;

typedef yaul_op_t yaul_op;


#endif

