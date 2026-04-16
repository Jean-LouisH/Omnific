/**
*                      This file is part of:
*                          Libretti
*
* File
*
* Handles input/output for scripting and file formats.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef File_h
#define File_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "binary.h"

bool exists(char* filename);
lb_BinaryU8 load_ascii_string_from_file(const char* filename);
lb_BinaryU8 load_binary_u8_from_file(const char* filename);
lb_BinaryS16 load_binary_s16_from_file(const char* filename);
void save_binary_u8_to_file(lb_BinaryU8* binary, const char* filename);
void save_binary_s16_to_file(lb_BinaryS16* binary, const char* filename);
void append_binary_u8_to_file(lb_BinaryU8* binary, const char* filename);
void append_binary_s16_to_file(lb_BinaryS16* binary, const char* filename);

#ifdef __cplusplus
}
#endif

#endif /*File_h*/