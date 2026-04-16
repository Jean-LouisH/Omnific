#include "strings.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

lb_String lb_new_string(const char* initial_string)
{
	lb_String string;
	string.length = strlen(initial_string);
	if (string.length == 0)
		string.capacity = 2;
	else
		string.capacity = pow(2.0, 1 + floor(log(string.length) / log(2.0)));
	string.data = calloc(string.capacity, sizeof(char));
	if (string.data != 0)
		strcpy(string.data, initial_string);
	return string;
}

void lb_append_string(lb_String* string, char symbol)
{
	if (string->data != 0)
	{
		if (string->length == string->capacity - 1)
		{
			int new_capacity = string->capacity * 2;
			string->data = realloc(string->data, new_capacity * sizeof(char));
			if (string->data != 0)
				string->capacity = new_capacity;
		}

		/*in case allocation fails, check again.*/
		if (string->length < string->capacity - 1)
		{
			string->length++;
			string->data[string->length - 1] = symbol;
			string->data[string->length] = 0;
		}
	}
}

void lb_clear_string(lb_String* string)
{
	if (string->data != 0)
	{
		string->data[0] = 0;
		string->length = 0;
	}
}

void lb_free_string(lb_String* string)
{
	if (string->data != 0)
	{
		free(string->data);
		string->length = 0;
		string->capacity = 0;
	}
}