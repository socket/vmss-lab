/*
 *  vmss_bitfields.c
 *  Lab2a
 *
 *  Created by Alexey Streltsow on 3/30/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "vmss_bitfields.h"
#include <stdio.h>

const char* bits2string(INTFIELD* fld) {
	static char sbits[128];
//	sbits[0] = fld->bits[0].v ? '1' : '0';
//	sbits[1] = ':';
	
	char *bs = sbits;
	
	int i;
	for ( i=0; i<BIT_COUNT; ++i) {
		*bs++ = fld->bits[i].v?'1':'0';
		if (!( (i+1) % 4) && i != 0 ){
			*bs++ = ' ';
		}
	}
	*bs = 0;
	
	return sbits;
}

void string2bits(const char* bitstring, INTFIELD* fld) {
	int i=0;
	const char* istr = bitstring;
	
	while (*bitstring) {
		if ( *bitstring == ' ' || *bitstring == '.' ) {
			bitstring++;
			continue;
		}
		fld->bits[++i].v = (*bitstring++ == '1') ? 1 : 0;
	}
	int count = BIT_COUNT - i;
	while (--count > 0) {
		shift_right(fld);
	}
	
	if (*istr == '-' ) {
		fld->bits[0].v = 1;
	}
}

void shift_left(INTFIELD* fld) {
	int i;
	for (i=0; i<BIT_COUNT-1; ++i) {
		fld->bits[i].v = fld->bits[i+1].v;
	}
	fld->bits[BIT_COUNT-1].v=0;
}

void shift_right(INTFIELD* fld) {
	int i;
	for (i=BIT_COUNT-1; i>=0; --i) {
		fld->bits[i].v = fld->bits[i-1].v;
	}
	fld->bits[0].v = 0;
}

void sum_bits(INTFIELD* b1, INTFIELD* b2, INTFIELD* res) {
	int add = 0;
	int i;
	for (i=BIT_COUNT-1; i>=0; --i) {
		if ( b1->bits[i].v == 0 && b2->bits[i].v == 0 ) {
			res->bits[i].v = add;
			add = 0;
		}
		else if ((b1->bits[i].v == 1 && b2->bits[i].v == 0) || 
						 (b1->bits[i].v == 0 && b2->bits[i].v == 1)) {
			res->bits[i].v = !add;
		}
		else if ( b1->bits[i].v == 1 && b2->bits[i].v == 1 ) {
			res->bits[i].v = add;
			add = 1;
		}
	}
}

void subtract_bits(INTFIELD* b1, INTFIELD* b2, INTFIELD* res) {
	int sub = 0;
	int i;
	for (i=BIT_COUNT-1; i>=0; --i) {
		if ( b1->bits[i].v == 0 && b2->bits[i].v == 0 ) {
			res->bits[i].v = sub;
		}
		else if ( b1->bits[i].v == 1 && b2->bits[i].v == 0 ) {
			res->bits[i].v = !sub;
			sub = 0;
		}
		else if ( b1->bits[i].v == 0 && b2->bits[i].v == 1 ) {
			res->bits[i].v = !sub;
			sub = 1;
		}
		else if ( b1->bits[i].v == 1 && b2->bits[i].v == 1 ) {
			res->bits[i].v = sub;
			//sub = 1;
		}
	}
}

void reverse_bits(INTFIELD* b, INTFIELD* res) {
	int i;
	for ( i=0; i<BIT_COUNT; i++ ) {
		res->bits[i].v = !b->bits[i].v;
	}
}

int bits2int(INTFIELD* b) {
	int value = 0;
	int i;
	for (i=1; i<BIT_COUNT; i++) {
		value |= (b->bits[i].v << (BIT_COUNT - i) );
	}
	//double fv = value / pow(10, b->p);
	return value;
}