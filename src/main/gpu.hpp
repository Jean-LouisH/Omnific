#pragma once

#ifdef _WIN32

#include <windows.h>
// Use discrete GPU by default.

extern "C" 
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif