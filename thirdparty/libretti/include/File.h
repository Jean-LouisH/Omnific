/**
*                      This file is part of:
*                          Libretti
*
* File
*
* Handles input/output for scripting and file formats.
*
* Copyright (c) 2017-2021 Jean-Louis Haywood. All rights reserved.
*/

#ifndef File_h
#define File_h

#include <stdbool.h>
#include "Binary.h"

bool exists(char* filename);
lb_BinaryU8 loadASCIIStringFromFile(const char* filename);
lb_BinaryU8 loadBinaryU8FromFile(const char* filename);
lb_BinaryS16 loadBinaryS16FromFile(const char* filename);
void saveBinaryU8ToFile(lb_BinaryU8* binary, const char* filename);
void saveBinaryS16ToFile(lb_BinaryS16* binary, const char* filename);
void appendBinaryU8ToFile(lb_BinaryU8* binary, const char* filename);
void appendBinaryS16ToFile(lb_BinaryS16* binary, const char* filename);

#endif /*File_h*/