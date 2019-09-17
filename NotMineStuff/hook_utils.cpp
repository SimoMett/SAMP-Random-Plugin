
//hook_utils.h not mine. Downloaded from https://github.com/dotSILENT/chandling/blob/master/chandling/hook_utils.h

#include "hook_utils.h"

bool memory_compare(const BYTE* data, const BYTE* pattern, const char* mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

DWORD FindPattern(DWORD module, const char* pattern, const char* mask)
{
	DWORD i;
	DWORD size;

	MODULEINFO info = { 0 };

	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("samp.dll"), &info, sizeof(MODULEINFO));
	size = (DWORD)info.SizeOfImage;

	for (i = 0; i < size; ++i)
	{
		if (memory_compare((BYTE*)(module + i), (BYTE*)pattern, mask))
			return (DWORD)(module + i);
	}
	return 0;
}