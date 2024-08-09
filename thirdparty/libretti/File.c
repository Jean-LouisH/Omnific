#include "include/File.h"
#include <stdio.h>
#include <stdlib.h>

bool exists(char* filename)
{
	FILE* inputFile = fopen(filename, "rb");
	if (inputFile != NULL)
		fclose(inputFile);
	return (bool)inputFile;
}

lb_BinaryU8 loadASCIIStringFromFile(const char* filename)
{
	FILE* inputFile = fopen(filename, "rb");
	lb_BinaryU8 string;
	string.data = NULL;
	string.size = 0;
	if (inputFile != NULL)
	{
		fseek(inputFile, 0, SEEK_END);
		string.size = ftell(inputFile);
		rewind(inputFile);
		string.data = malloc(sizeof *string.data * (string.size + 1));
		if (string.data != NULL)
		{
			fread(string.data, sizeof *string.data, string.size, inputFile);
			string.data[string.size] = NULL;
		}
		fclose(inputFile);
	}
	return string;
}

lb_BinaryU8 loadBinaryU8FromFile(const char* filename)
{
	FILE* inputFile = fopen(filename, "rb");
	lb_BinaryU8 binary;
	binary.data = NULL;
	binary.size = 0;
	if (inputFile != NULL)
	{
		fseek(inputFile, 0, SEEK_END);
		binary.size = ftell(inputFile);
		rewind(inputFile);
		binary.data = malloc(sizeof(uint8_t) * (binary.size));
		if (binary.data != NULL)
			fread(binary.data, sizeof(uint8_t), binary.size, inputFile);
		fclose(inputFile);
	}
	return binary;
}

lb_BinaryS16 loadBinaryS16FromFile(const char* filename)
{
	FILE* inputFile = fopen(filename, "rb");
	lb_BinaryS16 binary;
	binary.data = NULL;
	binary.size = 0;
	if (inputFile != NULL)
	{
		fseek(inputFile, 0, SEEK_END);
		binary.size = ftell(inputFile) / (sizeof *binary.data / sizeof(uint8_t));
		rewind(inputFile);
		binary.data = malloc(sizeof(int16_t) * (binary.size));
		int numberOfElements = 0;
		if (binary.data != NULL)
			numberOfElements = fread(binary.data, sizeof(int16_t), binary.size, inputFile);
		fclose(inputFile);
	}
	return binary;
}

void saveBinaryU8ToFile(lb_BinaryU8* binary, const char* filename)
{
	FILE* outputFile = fopen(filename, "wb");
	if (outputFile != NULL)
	{
		fwrite(binary->data, sizeof(uint8_t), binary->size, outputFile);
		fclose(outputFile);
	}
}

void saveBinaryS16ToFile(lb_BinaryS16* binary, const char* filename)
{
	FILE* outputFile = fopen(filename, "wb");
	if (outputFile != NULL)
	{
		fwrite(binary->data, sizeof(int16_t), binary->size, outputFile);
		fclose(outputFile);
	}
}

void appendBinaryU8ToFile(lb_BinaryU8* binary, const char* filename)
{
	FILE* outputFile = fopen(filename, "a");
	if (outputFile != NULL)
	{
		fwrite(binary->data, sizeof(uint8_t), binary->size, outputFile);
		fclose(outputFile);
	}
}

void appendBinaryS16ToFile(lb_BinaryS16* binary, const char* filename)
{
	FILE* outputFile = fopen(filename, "a");
	if (outputFile != NULL)
	{
		fwrite(binary->data, sizeof(int16_t), binary->size, outputFile);
		fclose(outputFile);
	}
}