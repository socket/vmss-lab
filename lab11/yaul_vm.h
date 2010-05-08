/*
 *  VM.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/9/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#ifndef _yaul_vm
#define _yaul_vm

#include "yaul_opcodes.h"

#define YAUL_CALL_DEPTH 256;
#define YAUL_DATA_STACK_SIZE  4096;


typedef enum {
	YVM_INTEGER,
	YVM_DOUBLE,
	YVM_STRING,
	YVM_FUNCTION
} yaul_type;

typedef struct  {
	const char		*_name;
	yaul_type			 _type;
	void					*_value;
} yaul_var_t;

typedef yaul_var_t yaul_var;

typedef struct {
	yaul_op				*_ret;
	yaul_var			*_vars;
} yaul_stack_t;

typedef yaul_stack_t yaul_stack;

typedef struct {
	void					*_data_stack;
	int						 _data_stack_sz;

	yaul_var		  *_globals;
	yaul_op				*_chunk;
	
	yaul_stack		*_call_stack;
	int						 _call_stack_sz;
} yaul_state_t;

typedef yaul_state_t yaul_state;

// YAUL API
void				yaul_open(yaul_state **Y);
void				yaul_close(yaul_state *Y);

void				yaul_dofile(yaul_state *Y, const char *filename);
void				yaul_dochunk(yaul_state *Y, yaul_op *code);

void			  yaul_get(yaul_state *Y, yaul_var **var, int pos);
void				yaul_pop(yaul_state *Y, int pos);
void				yaul_push(yaul_state *Y, yaul_var *var);

void				yaul_setglobal(yaul_state *Y);
void				yaul_getglobal(yaul_state *Y);

void				yaul_setcfunc(yaul_state *Y, const char *name, int (*func)(yaul_state* Y));

#endif