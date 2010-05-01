/*
 *  vmss_convert.h
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vmss_convert_h
#define _vmss_convert_h

#define min(a,b) a < b ? a : b

int vmss_convert2int(const char *src, int base, unsigned int *output);
int vmss_convert2str(unsigned int value, int base, char *output);
int vmss_rank10(unsigned int val);
int vmss_fractal2str(double fractal, int base, char* output, char limit);
int vmss_str2number(char *src, int src_base, int dst_base, char* output, char limit);
int vmss_str2double(const char* src, int src_base, double* out);
int vmss_double2str(double src, int dst_base, char* out, int limit);

#endif //_vmss_convert_h