/*
 *  vmss_common.h
 *  lab
 *
 *  Created by Alexey Streltsow on 3/5/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vmss_common_h
#define _vmss_common_h

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "vmss_string.h"
#include "vmss_convert.h"

typedef int BOOL;

#define TRUE 1
#define FALSE 0 

#define BUFF_SIZE 256
#define PRECISION 8

#define FAILED(x) ((-1) == x)
#define SUCCESS(x) !FAILED(x)

#endif