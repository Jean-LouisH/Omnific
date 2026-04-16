#include "file.h"
#include <stdio.h>
#include <stdlib.h>

bool exists(char* filename)
{
	FILE* input_file = fopen(filename, "rb");
	if (input_file != 0)
		fclose(input_file);
	return (bool)input_file;
}

lb_BinaryU8 load_ascii_string_from_file(const char* filename)
{
	FILE* input_file = fopen(filename, "rb");
	lb_BinaryU8 string;
	string.data = 0;
	string.size = 0;
	if (input_file != 0)
	{
		fseek(input_file, 0, SEEK_END);
		string.size = ftell(input_file);
		rewind(input_file);
		string.data = malloc(sizeof *string.data * (string.size + 1));
		if (string.data != 0)
		{
			fread(string.data, sizeof *string.data, string.size, input_file);
			string.data[string.size] = 0;
		}
		fclose(input_file);
	}
	return string;
}

lb_BinaryU8 load_binary_u8_from_file(const char* filename)
{
	FILE* input_file = fopen(filename, "rb");
	lb_BinaryU8 binary;
	binary.data = 0;
	binary.size = 0;
	if (input_file != 0)
	{
		fseek(input_file, 0, SEEK_END);
		binary.size = ftell(input_file);
		rewind(input_file);
		binary.data = malloc(sizeof(uint8_t) * (binary.size));
		if (binary.data != 0)
			fread(binary.data, sizeof(uint8_t), binary.size, input_file);
		fclose(input_file);
	}
	return binary;
}

lb_BinaryS16 load_binary_s16_from_file(const char* filename)
{
	FILE* input_file = fopen(filename, "rb");
	lb_BinaryS16 binary;
	binary.data = 0;
	binary.size = 0;
	if (input_file != 0)
	{
		fseek(input_file, 0, SEEK_END);
		binary.size = ftell(input_file) / (sizeof *binary.data / sizeof(uint8_t));
		rewind(input_file);
		binary.data = malloc(sizeof(int16_t) * (binary.size));
		int number_of_elements = 0;
		if (binary.data != 0)
			number_of_elements = fread(binary.data, sizeof(int16_t), binary.size, input_file);
		fclose(input_file);
	}
	return binary;
}

void save_binary_u8_to_file(lb_BinaryU8* binary, const char* filename)
{
	FILE* output_file = fopen(filename, "wb");
	if (output_file != 0)
	{
		fwrite(binary->data, sizeof(uint8_t), binary->size, output_file);
		fclose(output_file);
	}
}

void save_binary_s16_to_file(lb_BinaryS16* binary, const char* filename)
{
	FILE* output_file = fopen(filename, "wb");
	if (output_file != 0)
	{
		fwrite(binary->data, sizeof(int16_t), binary->size, output_file);
		fclose(output_file);
	}
}

void append_binary_u8_to_file(lb_BinaryU8* binary, const char* filename)
{
	FILE* output_file = fopen(filename, "a");
	if (output_file != 0)
	{
		fwrite(binary->data, sizeof(uint8_t), binary->size, output_file);
		fclose(output_file);
	}
}

void append_binary_s16_to_file(lb_BinaryS16* binary, const char* filename)
{
	FILE* output_file = fopen(filename, "a");
	if (output_file != 0)
	{
		fwrite(binary->data, sizeof(int16_t), binary->size, output_file);
		fclose(output_file);
	}
}