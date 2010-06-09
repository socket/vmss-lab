/*
 *  vmss_bitfields.h
 *  Lab2a
 *
 *  Created by Alexey Streltsow on 3/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vmss_bitfields
#define _vmss_bitfields

#define BIT_COUNT		32
#define POINT_POS		2

struct BITFIELD_T {
	unsigned v : 1;
};

typedef struct BITFIELD_T BITFIELD;

struct INTFIELD_T {
	BITFIELD bits[BIT_COUNT];
	unsigned int p;
};

typedef struct INTFIELD_T INTFIELD;

const char* bits2string(INTFIELD* fld);

void string2bits(const char* bitstring, INTFIELD* fld);
void shift_left(INTFIELD* fld);
void shift_right(INTFIELD* fld);
void sum_bits(INTFIELD* b1, INTFIELD* b2, INTFIELD* res);
void subtract_bits(INTFIELD* b1, INTFIELD* b2, INTFIELD* res);
void reverse_bits(INTFIELD* b, INTFIELD* res);

void sum_decbin(INTFIELD *b1, INTFIELD *b2, INTFIELD *res);

#endif