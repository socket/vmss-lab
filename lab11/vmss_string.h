/*
 *  vmss_string.h
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vmss_string_h
#define _vmss_string_h

void    vmss_strchop(char *str);
void    vmss_strrev(char *h);
char*   vmss_strend(char* str);
int     vmss_char2digit(char ch);
char    vmss_digit2char(int digit);
int			vmss_removedot(char* src, char* out);
void		vmss_addzeroes(char* num, int count);

#endif