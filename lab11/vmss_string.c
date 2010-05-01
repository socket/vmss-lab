/*
 *  vmss_string.c
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "vmss_string.h"

void vmss_strchop(char *str) {
  while (*str) {
    if ('\n' == *str || '\r' == *str) {
      *str = '\0';
      return;
    }
    str++;
  }
}

void vmss_strrev(char *h) {
  char *t = h;
  while(t && *t) ++t;
  for(--t; h < t; ++h, --t) {
    *h = *h ^ *t,
    *t = *h ^ *t,
    *h = *h ^ *t;
  }
}

char* vmss_strend(char* str) {
  while (*str) {
    str++;
  }
  return str;
}

int vmss_char2digit(char ch) {
  if ( ch >= '0' && ch <= '9' ) {
    return ch - '0';
  }
  if ( ch >= 'a' && ch <= 'z' ) {
    return 10 + (ch - 'a');
  }
  if ( ch >= 'A' && ch <= 'Z' ) {
    return 10 + (ch - 'A');
  }
  return -1;
};

char vmss_digit2char(int digit) {
  if ( digit >= 0 && digit <= 9 ) {
    return '0' + digit;
  }
  else if ( digit <= ('Z'-'A') ) {
    return 'A' + (digit - 10);
  }
  return '?';
}

int vmss_removedot(char* src, char* out) {
	int cnt = 0;
	int dot = 0;
	while (*src) {
		cnt++;
		if ( *src == '.' ) {
			src++;
			dot = cnt;
			continue;
		}
		*out++ = *src++;
	}
	*out = 0;
	
	return cnt-dot;
}

void vmss_addzeroes(char* num, int count) {
	while(*num) num++;
	while (count--) {
		*num++ = '0';
	}
	*num++ = '.';
	*num = 0;
}