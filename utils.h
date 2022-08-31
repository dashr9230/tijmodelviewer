
#pragma once

#define DEBUG_TRACE(x) DebugLog(x)

void InstallCallHook(DWORD dwAddress, DWORD dwFunction);
void DebugLog(const char* format, ...);
