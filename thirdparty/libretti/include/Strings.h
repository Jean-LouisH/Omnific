/**
*                      This file is part of:
*                          Libretti
*
* Strings
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

typedef struct
{
	char* data;
	int length;
	int capacity;
}lb_String;

lb_String lb_newString(const char* initialString);
void lb_appendString(lb_String* string, char symbol);
void lb_clearString(lb_String* string);
void lb_freeString(lb_String* string);