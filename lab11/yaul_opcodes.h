/*
 *  yaul_opcodes.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

typedef enum { 
	OP_PUSH_INT,
	OP_PUSH_DOUBLE,
	OP_PUSH_STRING,
	
	OP_POP,
	OP_RETN,
	
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_BIN_AND,
	OP_BIN_OR,
	OP_NOT,
	
	OP_EQ,
	OP_NEQ,
	OP_LEQ,
	OP_GEQ,
	OP_GREATER,
	OP_LESSER,
	
	OP_CMP,
	
	OP_JMP,
	
	OP_JEQ,
	OP_JNE,
	
	OP_CALLFUNC,
	OP_CALLNATIVE,
	
	OP_GETLOCAL,
	OP_SETLOCAL,
	OP_GETGLOBAL,
	OP_SETGLOBAL,
	
	OP_NOP,
	
	OP_4BYTE_PADDING = 0xFFFFFFFF
} yaul_opcode;

typedef struct  {
	yaul_opcode	_opcode;
	void			 *_operand;
} yaul_op_t;

typedef yaul_op_t yaul_op;
