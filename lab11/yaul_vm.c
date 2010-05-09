/*
 *  VM.cpp
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "yaul_vm.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void yaul_open(yaul_state **Y) {
	assert( Y ) ;
	*Y = (yaul_state*)malloc(sizeof(yaul_state));
	yaul_state *ys = *Y;
	memset(ys, 0, sizeof(yaul_state));
	
	ys->_data_stack = (yaul_var*)malloc(YAUL_DATA_STACK_SIZE*sizeof(yaul_var));
	memset(ys->_data_stack, 0, YAUL_DATA_STACK_SIZE*sizeof(yaul_var));
	ys->_dsz = YAUL_DATA_STACK_SIZE;
	ys->_dpos = 0;
	
	ys->_globals = (yaul_var*)calloc(32, sizeof(yaul_var));
	ys->_gsz = 32;
}

void yaul_close(yaul_state *Y) {
	assert(Y);
	free(Y);
}

void yaul_dofile(yaul_state *Y, const char *filename) {
	
}

void yaul_dochunk(yaul_state *Y, yaul_op *code, int chunk_size) {
	Y->_chunk = code;
	Y->_csz = chunk_size;
	Y->_cpos = 0;
	
	while( Y->_cpos < Y->_csz ) {
		yaul_op *op = Y->_chunk + Y->_cpos;
		assert( op );
		
		int res =  yaulvm_exec_op(Y, op);
		if ( res < 0 ) {
			yaul_var *err;
			yaul_get(Y, &err, 0);
			printf("yaulVM: %s", (const char*)err->_value);
			return;
		}
		else if ( res == 1 ) {
			break; // retn
		}
	}
}

int yaulvm_exec_op(yaul_state *Y, yaul_op *o) {
	assert(Y);
	assert(o);
	
	if ( Y->_dpos >= Y->_dsz ) {
		Y->_dsz *= 2;
		Y->_data_stack = realloc(Y->_data_stack, Y->_dsz);
	}
	
	switch (o->_opcode) {
		case OP_PUSH_STRING:
			yaul_push_string(Y, (const char*) o->_operand);
			break;
		
		case OP_PUSH_INT:
			yaul_push_int(Y, o->_operand);
			break;
			
		case OP_PUSH_DOUBLE:
			Y->_data_stack[Y->_dpos]._type = YVM_DOUBLE;
			Y->_data_stack[Y->_dpos++]._value = o->_operand;			
			break;
			
		case OP_RETN:
			return 1;
		
		case OP_POP:
			yaul_pop(Y, o->_operand);
			break;
			
		case OP_NOP:
			break;
			
		case OP_JNE: // jump if flag not set
			if (Y->_cmp_flag ) {
				Y->_cmp_flag = 0;
				break;
			}

		case OP_JMP: // uncond jump
			if ( Y->_cpos + o->_opcode <= Y->_csz ) {
				Y->_cpos += o->_operand;
			}
			else {
				yaul_push_string(Y, "Jump out of bounds");
				return -1;
			}
			break;
			
		case OP_CMP:
			
			
		case OP_CALLFUNC:
		
		case OP_GETGLOBAL:
		case OP_SETGLOBAL:
		
			
		default:
			break;
	}
}

void yaul_push_int(yaul_state *Y, int value) {
	Y->_data_stack[Y->_dpos]._type = YVM_INTEGER;
	Y->_data_stack[Y->_dpos]._value = value;
	Y->_dpos++;
}

void yaul_push_string(yaul_state *Y, const char* value) {
	Y->_data_stack[Y->_dpos]._type = YVM_STRING;
	Y->_data_stack[Y->_dpos]._value = (long)malloc(strlen(value)+1);
	memcpy((void*)Y->_data_stack[Y->_dpos]._value, value, strlen(value)+1 );
	Y->_dpos++;
}

void yaul_push(yaul_state *Y, yaul_var *var) {
	assert(var);
	Y->_data_stack[Y->_dpos] = *var;
	Y->_dpos++;
}

void yaul_get(yaul_state *Y, yaul_var **var, int pos) {
	if ( pos > 0 ) {
		pos = -pos;
	}
	if ( Y->_dsz - 1 + pos > 0 ) {
		*var = Y->_data_stack[Y->_dsz- 1 + pos];
	}
	else {
		*var = 0;
	}
}

void yaul_pop(yaul_state *Y, int n) {
	while(n-- > 0) {
		if ( Y->_data_stack[Y->_dpos-1]._type == YVM_STRING ) {
			free((void*)Y->_data_stack[Y->_dpos-1]._value);
		}
		Y->_dpos--;	
	}
}

void yaul_setglobal(yaul_state *Y) {
	
}

void yaul_getglobal(yaul_state *Y) {
	
}

void yaul_setcfunc(yaul_state *Y, const char *name, int (*func)(yaul_state* Y)) {
	
}
