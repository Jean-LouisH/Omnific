/**
*                      This file is part of:
*                          Libretti
*
* Strings
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef Strings_h
#define Strings_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	char* data;
	int length;
	int capacity;
}lb_String;

lb_String lb_new_string(const char* initial_string);
void lb_append_string(lb_String* string, char symbol);
void lb_clear_string(lb_String* string);
void lb_free_string(lb_String* string);

#ifdef __cplusplus
}
#endif

#endif /*Strings_h*/