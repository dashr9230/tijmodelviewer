
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>

#include "utils.h"

void InstallCallHook(DWORD dwAddress, DWORD dwFunction)
{
	*(PBYTE)(dwAddress) = 0xE8;
	*(PDWORD)(dwAddress + 1) = dwFunction - (dwAddress + 5);
}

void NOP(DWORD dwAddress, DWORD dwSize)
{
	memset((PVOID)dwAddress, 0x90, dwSize);
}

void Log(const char* format, ...)
{
	FILE* f;
	fopen_s(&f, "tijmodelviewer.log", "a");
	if (!f) return;

	va_list args;
	va_start(args, format);
	vfprintf(f, format, args);
	fputs("\n", f);
	va_end(args);

	fclose(f);
}
