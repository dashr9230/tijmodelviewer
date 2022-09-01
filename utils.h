
#pragma once

void InstallCallHook(DWORD dwAddress, DWORD dwFunction);
void NOP(DWORD dwAddress, DWORD dwSize);
void Log(const char* format, ...);
