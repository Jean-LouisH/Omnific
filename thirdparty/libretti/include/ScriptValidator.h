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

#include <stdbool.h>
#include <stdint.h>

int validateScript(const char* script);
bool validateSymbol(char symbol, uint8_t parseState);

#endif /*ScriptValidator_h*/