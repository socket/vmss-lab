/*
 *  vmss_convert.c
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "vmss_common.h"

int vmss_convert2int(const char *src, int base, unsigned int *output) {
  int i = 0;
  const char *src_end = src;
  
  *output = 0;
  while (*src_end) src_end++;
  
  while(*src++) {
    char digit = vmss_char2digit( *(--src_end) );
    if ( digit < 0 ) {
      return -1;
    }
    if ( digit >= base ) {
      return -2;
    }
    *output += digit*pow(base, i++);
  }
  return 0;
}

int vmss_fconvert2int(const char *src, int base, double *out) {
  int i = 1;
  const char *src_end = src;
  double output = 0;
	
  *out = 0;
  while (*src_end) src_end++;
  
  while(*src) {
    char digit = vmss_char2digit( *src );
    if ( digit < 0 ) {
      return -1;
    }
    if ( digit >= base ) {
      return -2;
    }
    output += digit*pow(base, (-1)*i++);
		src++;
	}
	*out = output;
	
  return 0;
}


int vmss_convert2str(unsigned int value, int base, char *output) {
  char* pstr = output;
  do {
    int remainder = value % base;
    value /= base;
    *pstr++ = vmss_digit2char(remainder);
  }
  while (value > 0);
  
  *pstr = '\0';
  vmss_strrev(output);
  
  return 0;
}

int vmss_rank10(unsigned int val) {
  int rank = 0;
	while(val=val/10) rank++;
	return rank+1;
}

int vmss_fractal2str(double fractal, int base, char* output, char limit) {
	int i;
	
  for (i=0; i<limit && fractal > 0; ++i) {
		double fractp, intp;
		fractal *= base;
		fractp = modf(fractal, &intp);
		*output++ = vmss_digit2char( (int)intp );
		fractal = fractp;
	}
	
  *output = '\0';
  return 0;
}

int 
vmss_str2number(char *src, int src_base, int dst_base, char* output, char limit) {
  char *left, *right;
  unsigned int src_value;
	double src_fractal = 0;
	
  int result = 0;
  	
  if ( *src == '-' ) {
    *output++ = *src++;
  }
  
  left = right = src;
  while(*right && *right != '.') right++;
  *right++ = '\0';
  
  result |= vmss_convert2int(left, src_base, &src_value);
  result |= vmss_fconvert2int(right, src_base, &src_fractal);
  
  if ( result != 0 ) {
    return -1;
  }
  
  vmss_convert2str(src_value, dst_base, output);
  output = vmss_strend( output );
  *output++ = '.';
  vmss_fractal2str(src_fractal, dst_base, output, limit);
  
  return 0;
}

int 
vmss_str2double(const char* csrc, int src_base, double* out) {
  char *left, *right;
  unsigned int src_value;
	double src_fractal = 0;
	
	char bsrc[256];
	char *src = bsrc;
	strcpy(bsrc, csrc);

	int result = 0;
	double sign = 1;
	
  if ( *src == '-' ) {
		sign = -1;
		src++;
  }
  
  left = right = src;
  while(*right && *right != '.') right++;
  *right++ = '\0';
  
  result |= vmss_convert2int(left, src_base, &src_value);
  result |= vmss_fconvert2int(right, src_base, &src_fractal);
 	
	if ( result != 0 ) {
    return -1;
  }
  
	*out = (1.0f*src_value + src_fractal);
	*out = *out * sign;
	
	return 0;
}

int 
vmss_double2str(double src, int dst_base, char* out, int limit) {
	if ( src < 0 ) {
		src *= -1.0f;
		*out++ = '-';
	}
	
	double src_value;
	double src_fractal = modf(src, &src_value);
	
	vmss_convert2str((int)src_value, dst_base, out);
  out = vmss_strend( out );
  *out++ = '.';
  vmss_fractal2str(src_fractal, dst_base, out, limit);
	
	return 0;
}
