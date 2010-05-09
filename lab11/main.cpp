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

int main (int argc, char * const argv[]) {
	Lexer lex;
	Parser parser;
	LexToken *tokens;
	int count;
	
	//lex.analyze("if (test > 5) then do_something end", &tokens, &count);
	
	FILE *file = fopen("/Users/socket/University/vmss-lab/lab11/test.yauc", "r");
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
				gen.generate(node, &code);
				gen.print(stdout);
			}
			
		}
		fclose(file);
	}
	else {
		printf("file not found");
	}

	
	
	return 0;
}
