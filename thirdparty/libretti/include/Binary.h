/**
*                      This file is part of:
*                          Libretti
*
* Binary
*
* Defines the basic data structure of incoming and outgoing file binaries.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
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
}lb_BinaryU8;

typedef struct
{
	uint16_t* data;
	uint64_t size;
}lb_BinaryU16;

typedef struct
{
	uint32_t* data;
	uint64_t size;
}lb_BinaryU32;

//Signed binaries
//////////////////////////////////

typedef struct
{
	int8_t* data;
	uint64_t size;
}lb_BinaryS8;

typedef struct
{
	int16_t* data;
	uint64_t size;
}lb_BinaryS16;

typedef struct
{
	int32_t* data;
	uint64_t size;
}lb_BinaryS32;

#endif /*Binary_h*/