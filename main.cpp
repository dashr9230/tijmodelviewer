
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include "rwcore.h"
#include "utils.h"

RsGlobalType& RsGlobal = *(RsGlobalType*)0x004EE9E0;
RwBool& RwInitialized = *(RwBool*)0x004E7138;
RwBool& ForegroundApp = *(RwBool*)0x004DB4B4;

LRESULT CALLBACK MainWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				SetTimer(window, 1, 20, NULL);
				ForegroundApp = FALSE;

				RsEventHandler(rsACTIVATE, (void*)FALSE);
			}
			else
			{
				KillTimer(window, 1);
				ForegroundApp = TRUE;
				RsEventHandler(rsACTIVATE, (void*)TRUE);
			}
			return 0L;
		}

		case WM_TIMER:
		{
			if (RwInitialized)
			{
				RsEventHandler(rsIDLE, NULL);
			}

			return 0L;
		}

		case WM_CLOSE:
		case WM_DESTROY:
		{
			ClipCursor(NULL);

			PostQuitMessage(0);

			return 0L;
		}
	}

	return DefWindowProc(window, message, wParam, lParam);
}

INT WINAPI WinMain_HOOK(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	RwV2d pos;
	MSG message;

	DebugLog("WinMain_HOOK start\n");

	strncpy((char*)0x004CF8A4, "tijmodelviewer", 16);

	if (RsEventHandler(rsINITIALIZE, NULL) == rsEVENTERROR)
	{
		DebugLog("rsINITIALIZE event failed.\n");
		return FALSE;
	}

	// Detour game's MainWndProc to ours from InitApplication (0x00431F30)
	*(DWORD*)0x00431F40 = (DWORD)MainWndProc;
	
	if (!InitApplication(hInstance))
	{
		DebugLog("InitApplication failed.\n");
		return FALSE;
	}

	// RsEventHandler does not handle rsPREINITCOMMANDLINE (25) event
	//RsEventHandler(rsPREINITCOMMANDLINE, lpCmdLine);

	//PSGLOBAL(fullScreen) = TRUE;

	// Restore window frame
	*(DWORD*)0x00431FC8 = WS_OVERLAPPEDWINDOW;
	*(DWORD*)0x00431FF3 = WS_OVERLAPPEDWINDOW;

	PSGLOBAL(window) = InitInstance(hInstance);

	SetForegroundWindow(PSGLOBAL(window));

	if (PSGLOBAL(window) == NULL)
	{
		DebugLog("InitInstance failed.\n");
		return FALSE;
	}

	PSGLOBAL(instance) = hInstance;

	if (rsEVENTERROR == RsEventHandler(rsRWINITIALIZE, PSGLOBAL(window)))
	{
		DebugLog("rsRWINITIALIZE event failed.\n");

		DestroyWindow(PSGLOBAL(window));

		//RsEventHandler(rsTERMINATE, NULL); // does nothing

		return FALSE;
	}

	RwInitialized = TRUE;

	// Not required? RsEventHandler doest not handle the rsFILELOAD also...
	//RsEventHandler(rsCOMMANDLINE, lpCmdLine);

	ShowWindow(PSGLOBAL(window), nCmdShow);
	UpdateWindow(PSGLOBAL(window));

	RsAlwaysOnTop(TRUE);

	pos.x = RsGlobal.maximumWidth * 0.5f;
	pos.y = RsGlobal.maximumHeight * 0.5f;

	RsMouseSetPos(&pos);

	while (!RsGlobal.quit)
	{
		if (PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE | PM_NOYIELD))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else if (ForegroundApp)
		{
			if (FALSE != RwInitialized)
			{
				RsEventHandler(rsIDLE, NULL);
			}
		}
		else
		{
			WaitMessage();
		}
	}

	RwInitialized = FALSE;

	RsAlwaysOnTop(FALSE);

	RsEventHandler(rsRWTERMINATE, NULL);

	DestroyWindow(PSGLOBAL(window));

	//RsEventHandler(rsTERMINATE, NULL); // does nothing

	DebugLog("WinMain_HOOK end\n");

	return message.wParam;
}

void RemovePageProtection()
{
	DWORD dwOldProt;
	if (VirtualProtect((LPVOID)0x00401000, 0x10D37F, PAGE_EXECUTE_READWRITE, &dwOldProt) == FALSE)
	{
		DEBUG_TRACE("VirtualProtect failed.\n");
	}
}

BOOL CheckExecutable()
{
	DWORD dwModule = (DWORD)GetModuleHandle("stg089.exe");

	if (dwModule == 0)
	{
		return FALSE;
	}

	// 8A 90 61 11 40 00
	// .text:00401044		mov     dl, ds:byte_401161[eax]
	PBYTE pData = (PBYTE)(dwModule + 0x1044);

	if( pData[0] != 0x8A &&
		pData[1] != 0x90 &&
		pData[2] != 0x61 &&
		pData[3] != 0x11 &&
		pData[4] != 0x40 &&
		pData[4] != 0x00)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);

		if (CheckExecutable() == FALSE)
		{
			MessageBoxA(NULL, "Bad The Italian Job stage executable.", "Error", MB_ICONERROR | MB_OK);
			ExitProcess(1);
			return FALSE;
		}

		RemovePageProtection();

		InstallCallHook(0x43263B, (DWORD)WinMain_HOOK);
	}
	return TRUE;
}
