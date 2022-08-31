
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>

#include "utils.h"

void InstallCallHook(DWORD dwAddress, DWORD dwFunction)
{
	*(PBYTE)(dwAddress) = 0xE8;
	*(PDWORD)(dwAddress + 1) = dwFunction - (dwAddress + 5);
}

void DebugLog(const char* format, ...)
{
	FILE* f;
	fopen_s(&f, "tijmodelviewer_debug.log", "a");
	if (!f) return;

	va_list args;
	va_start(args, format);
	vfprintf(f, format, args);
	va_end(args);

	fclose(f);
}
