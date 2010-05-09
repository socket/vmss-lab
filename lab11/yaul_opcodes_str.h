/*
 *  yaul_opcodes_str.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

static const char* opcodes_str[] = { 
	"NULL",
	
	"OP_PUSH_INT",
	"OP_PUSH_DOUBLE",
	"OP_PUSH_STRING",
	
	"OP_POP",
	"OP_RETN",
	
	"OP_ADD",		// pop 2 push 1
	"OP_SUB",
	"OP_MUL",
	"OP_DIV",
	"OP_BIN_AND",
	"OP_BIN_OR",
	"OP_NOT",
	"OP_XOR",
	
	"OP_EQ",
	"OP_NEQ",
	"OP_LEQ",
	"OP_GEQ",
	"OP_GREATER",
	"OP_LESSER",
	
	"OP_CMP",
	
	"OP_JMP",
	
	"OP_JNE",
	
	"OP_CALLFUNC",
	
	"OP_GETGLOBAL",
	"OP_SETGLOBAL",
	
	"OP_NOP"
};