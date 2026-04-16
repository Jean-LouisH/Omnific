/**
*                      This file is part of:
*                          Libretti
*
* Script Validator
*
* Ensures the Libretti script file is valid according to the
* language specification before it is compiled.
*
* Copyright (c) 2017 Jean-Louis Haywood. All rights reserved.
*/

#ifndef ScriptValidator_h
#define ScriptValidator_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

int validate_script(const char* script);
bool validate_symbol(char symbol, uint8_t parse_state);

#ifdef __cplusplus
}
#endif

#endif /*ScriptValidator_h*/