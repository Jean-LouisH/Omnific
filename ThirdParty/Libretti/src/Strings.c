#include "Strings.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

lb_String lb_newString(const char* initialString)
{
	lb_String string;
	string.length = strlen(initialString);
	if (string.length == 0)
		string.capacity = 2;
	else
		string.capacity = pow(2.0, 1 + floor(log(string.length) / log(2.0)));
	string.data = calloc(string.capacity, sizeof(char));
	if (string.data != NULL)
		strcpy(string.data, initialString);
	return string;
}

void lb_appendString(lb_String* string, char symbol)
{
	if (string->data != NULL)
	{
		if (string->length == string->capacity - 1)
		{
			int newCapacity = string->capacity * 2;
			string->data = realloc(string->data, newCapacity * sizeof(char));
			if (string->data != NULL)
				string->capacity = newCapacity;
		}

		/*in case allocation fails, check again.*/
		if (string->length < string->capacity - 1)
		{
			string->length++;
			string->data[string->length - 1] = symbol;
			string->data[string->length] = NULL;
		}
	}
}

void lb_clearString(lb_String* string)
{
	if (string->data != NULL)
	{
		string->data[0] = NULL;
		string->length = 0;
	}
}

void lb_freeString(lb_String* string)
{
	if (string->data != NULL)
	{
		free(string->data);
		string->length = 0;
		string->capacity = 0;
	}
}