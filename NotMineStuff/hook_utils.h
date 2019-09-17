
//hook_utils.h not mine. Downloaded from https://github.com/dotSILENT/chandling/blob/master/chandling/hook_utils.h

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

bool memory_compare(const BYTE* data, const BYTE* pattern, const char* mask);

DWORD FindPattern(DWORD module, const char* pattern,const char* mask);