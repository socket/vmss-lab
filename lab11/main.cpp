/*
 *  vmss_string.h
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

// 23. 4 to 16

#include "vmss_common.h"
#include "yaul_lexer.h"
#include "yaul_parser.h"
#include "yaul_codegen.h"

#include "yaul_vm.h"

#include "vmss_decbin.h"
	
int main2 (int argc, char * const argv[]) {
	INTFIELD a, b, res;
	
	vmss_int2decbin(534, &a);
	vmss_int2decbin(540, &b);
	
	vmss_decbinsum(&a, &b, &res);
	
	int result = vmss_decbin2int(&res);
	printf("DEC: %d\n", result);
	
	return 0;
}

int vmss_decsum(yaul_state *Y) {
	INTFIELD a, b, res;
	
	yaul_var *var1, *var2;
	yaul_get(Y, &var1, -1);
	yaul_get(Y, &var2, 0);
	
	int ca = var1->_value;
	int cb = var2->_value;
	
	vmss_int2decbin(ca, &a);
	vmss_int2decbin(cb, &b);
	
	vmss_decbinsum(&a, &b, &res);
	
	int result = vmss_decbin2int(&res);
	//printf("%d + %d = %d", ca, cb, result);
	
	yaul_var v;
	v._value = result;
	v._type = YVM_INTEGER;
	
	yaul_pop(Y, 2);
	
	yaul_push(Y, &v);
	
	return 0;
}


int main (int argc, char * const argv[]) {
	Lexer lex;
	Parser parser;
	LexToken *tokens;
	int count;
	
	FILE *file = fopen("../../test.yauc", "r");
	if (file) {
		char *buff;
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fseek(file, 0, SEEK_SET);
		if ( size > 0 ) {
			buff = (char*)malloc(size+16);
			memset(buff, 0, size+16);
			fread(buff, 1, size, file);
			lex.analyze(buff, &tokens, &count);
			parser.init(tokens);
			ParseNode *node;
			if ( parser.parse( &node ) ) {
				printf("*** Parse OK\n");
				parser.saveDebugTree(stdout);
			
				CodeGen gen;
				yaul_op* code;
				int size = 0;
				gen.generate(node, &code, &size);
				gen.print(stdout);
				
				yaul_state *Y;
				yaul_open(&Y);
				yaul_setcfunc(Y, "vmss_decsum", vmss_decsum);
				printf("**************\n");
				yaul_dochunk(Y, code, size);
				
			}
			
		}
		fclose(file);
	}
	else {
		printf("file not found");
	}

	
	
	return 0;
}



