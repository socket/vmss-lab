/*
 *  vmss_decbin.h
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vmss_decbin_h

#define _vmss_decbin_h


#ifdef __cplusplus
extern "C" {
#endif
	
#include "vmss_common.h"
#include "vmss_bitfields.h"

typedef struct {
	BITFIELD bit[4];
} bit_tetrad_t;

typedef bit_tetrad_t bit_tetrad;

void vmss_decbinsum(INTFIELD *b1, INTFIELD *b2, INTFIELD *res);
void vmss_int2decbin(int val, INTFIELD *f);

int vmss_decbin2int(INTFIELD *f);
	
	
#ifdef __cplusplus
}
#endif
		
#endif // _vmss_decbin_h