/**
*                      This file is part of:
*                          Libretti
*
* Binary
*
* Defines the basic data structure of incoming and outgoing file binaries.
*
* Copyright (c) 2017-2019 Jean-Louis Haywood. All rights reserved.
* License: https://github.com/Jean-LouisH/Libretti/blob/master/LICENSE
*/

#ifndef Binary_h
#define Binary_h

#include <stdint.h>

//Unsigned binaries
//////////////////////////////////

typedef struct
{
	uint8_t* data;
	uint64_t size;
}lb_Binary_u8;

typedef struct
{
	uint16_t* data;
	uint64_t size;
}lb_Binary_u16;

typedef struct
{
	uint32_t* data;
	uint64_t size;
}lb_Binary_u32;

//Signed binaries
//////////////////////////////////

typedef struct
{
	int8_t* data;
	uint64_t size;
}lb_Binary_s8;

typedef struct
{
	int16_t* data;
	uint64_t size;
}lb_Binary_s16;

typedef struct
{
	int32_t* data;
	uint64_t size;
}lb_Binary_s32;

#endif /*Binary_h*/