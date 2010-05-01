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
#include "Lexer.h"

int main (int argc, char * const argv[]) {
	Lexer lex;
	LexToken* tokens;
	int count;
	
	lex.analyze(".2.3.4.5.6.8", &tokens, &count);
	
	return 0;
}
