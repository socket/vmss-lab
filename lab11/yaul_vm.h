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

#define YAUL_CALL_DEPTH 256
#define YAUL_DATA_STACK_SIZE  4096


typedef enum {
	YVM_INTEGER,
	YVM_DOUBLE,
	YVM_STRING,
	YVM_FUNCTION,
	YVM_CFUNCTION
} yaul_type;

typedef struct  {
	const char		*_name;
	yaul_type			 _type;
	long					 _value;
} yaul_var_t;

typedef yaul_var_t yaul_var;

typedef struct {
	yaul_op				*_ret;
	yaul_var			*_vars;
} yaul_stack_t;

typedef yaul_stack_t yaul_stack;

typedef struct {
	yaul_var			*_data_stack;
	int						 _dsz;
	int						 _dpos;
	
	yaul_var		  *_globals;
	int						_gsz;
	yaul_op				*_chunk;
	int						_csz;
	int						_cpos;
	
	int						_cmp_flag;
	
	yaul_stack		*_call_stack;
	int						 _call_stack_sz;
} yaul_state_t;

typedef yaul_state_t yaul_state;

// YAUL API
void				yaul_open(yaul_state **Y);
void				yaul_close(yaul_state *Y);

void				yaul_dofile(yaul_state *Y, const char *filename);
void				yaul_dochunk(yaul_state *Y, yaul_op *code, int chunk_size);

void			  yaul_get(yaul_state *Y, yaul_var **var, int pos);
void				yaul_pop(yaul_state *Y, int n);
void				yaul_push(yaul_state *Y, yaul_var *var);
void				yaul_push_int(yaul_state *Y, int value);
void				yaul_push_string(yaul_state *Y, const char* value);

void				yaul_setglobal(yaul_state *Y);
void				yaul_getglobal(yaul_state *Y);

void				yaul_setcfunc(yaul_state *Y, const char *name, int (*func)(yaul_state*));

int				yaulvm_exec_op(yaul_state *Y, yaul_op *o);

#endif