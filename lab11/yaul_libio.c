/*
 *  yaul_libio.c
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "yaul_libio.h"
#include <stdlib.h>
#include <stdio.h>

void yaul_libio_open(yaul_state *Y) {
	yaul_setcfunc(Y, "print", &yaul_libio_print);
}

int yaul_libio_print(yaul_state *Y) {
	yaul_var *var;
	yaul_get(Y, &var, 0);
	
	if ( var ) {
		if ( var->_type == YVM_STRING ) {
			printf("%s", (const char*)var->_value);
		}
		else if ( var->_type == YVM_INTEGER ) {
			printf("%d", (int)var->_value);			
		}
		else {
			printf("(error)");
		}
	}
	
	yaul_pop(Y, 1);
	return 0;
}