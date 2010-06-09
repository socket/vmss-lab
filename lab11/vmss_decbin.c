/*
 *  vmss_decbin.c
 *  lab11
 *
 *  Created by Alexey Streltsow on 5/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "vmss_decbin.h"
#include <assert.h>

void vmss_gettet(INTFIELD *f, int n, bit_tetrad *t) {
	int i;
	for (i=0; i<4; ++i) {
		t->bit[i] = f->bits[4*n + i];
	}
}

int vmss_tet2int(bit_tetrad *t) {
	int i, val = 0;
	for (i=0; i<4; ++i) {
		val |= (t->bit[3-i].v << i);
	}
	return val;
}

void vmss_clearbits(INTFIELD *f) {
	int i;
	for (i=0; i<BIT_COUNT; ++i) {
		f->bits[i].v = 0;
	}
}

void vmss_int2decbin(int val, INTFIELD *f) {
	int i=(BIT_COUNT) - 4;
	vmss_clearbits(f);
	while( val > 0 ) {
		int digit = val % 10;
		
		if ( i <= 4 ) {
			assert(0);
			return; //overflow
		}
		
		f->bits[i].v = digit / 8;
		f->bits[i+1].v = (digit / 4) % 2;
		f->bits[i+2].v = (digit / 2) % 2;
		f->bits[i+3].v = digit % 2;
		
		i -= 4;
		
		val = val / 10;
	}
}

int vmss_decbin2int(INTFIELD *f) {
	int tets = BIT_COUNT/4; //total tetrads
	int i, k, res = 0;
	k=0;
	for ( i=tets-1; i>=0; --i, ++k ) {
		bit_tetrad tetr;
		vmss_gettet(f, i, &tetr);
		int val = vmss_tet2int(&tetr) ;		
		res += val*pow(10, k);
	}
	return res;
}

// correcting tetrad
void vmss_makefixtet(INTFIELD *src, INTFIELD *fix) {
	int i;
	int null = 1;
	vmss_clearbits(fix);
	for ( i=0; i<BIT_COUNT/4; ++i ) {
		bit_tetrad tetr;
		vmss_gettet(src, i, &tetr);
		if ( vmss_tet2int(&tetr) != 0 || !null ) {
			fix->bits[4*i + 0].v = 0;
			fix->bits[4*i + 1].v = 1;
			fix->bits[4*i + 2].v = 1;
			fix->bits[4*i + 3].v = 0;			
			null = 0;
		}
	}
}

// 2nd correcting tetrad
void vmss_makefixtet_sub(INTFIELD *src, INTFIELD *fix, int count) {
	int i;
	
	vmss_clearbits(fix);
	for ( i=BIT_COUNT/4-count; i<BIT_COUNT/4; ++i ) {
		bit_tetrad tetr;
		vmss_gettet(src, i, &tetr);
		if ( tetr.bit[0].v == 0 ) {
			fix->bits[4*i + 0].v = 1;
			fix->bits[4*i + 1].v = 0;
			fix->bits[4*i + 2].v = 1;
			fix->bits[4*i + 3].v = 0;			
		}
	}
}

int vmss_gettet_count(INTFIELD *src) {
	int i;
	for ( i=0; i<BIT_COUNT/4; ++i ) {
		bit_tetrad tetr;
		vmss_gettet(src, i, &tetr);
		if ( vmss_tet2int(&tetr) != 0 ) {
			break;
		}
	}
	return BIT_COUNT/4 - i;
}

void vmss_tsum(INTFIELD *b1, INTFIELD *b2, INTFIELD *res, INTFIELD *transfer) {
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
		if ( transfer ) {
			transfer->bits[i].v = add;
		}
	}
}


void vmss_sum_notf(INTFIELD *b1, INTFIELD *b2, INTFIELD *res) {
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
		if ( 0 == (i % 4) )
			add = 0;
		
	}
}

void vmss_decbinsum(INTFIELD *b1, INTFIELD *b2, INTFIELD *res) {
	INTFIELD cr, cr2, res1, res2, tf;
	int tet_count;
	// make correction
	printf("A:  %s\n", bits2string(b1));
	printf("B:  %s\n", bits2string(b2));
	vmss_makefixtet(b1, &cr);
	printf("F:  %s\n", bits2string(&cr));
	// sum A + CR = A'
	vmss_tsum(b1, &cr, &res1, 0);
	printf("A': %s\n", bits2string(&res1));
	// sum A' + B = B'
	vmss_tsum(&res1, b2, &res2, &tf);
	printf("B:  %s\n", bits2string(b2));
	printf("B': %s\n", bits2string(&res2));
	// 2nd correction 
	printf("TF: %s\n", bits2string(&tf));
	tet_count = vmss_gettet_count(&cr);
	vmss_makefixtet_sub(&tf, &cr2, tet_count);
	printf("F': %s\n", bits2string(&cr2));
	// B' + CR' = B
	vmss_sum_notf(&res2, &cr2, res);
	printf("RS: %s\n", bits2string(res));
	
}
