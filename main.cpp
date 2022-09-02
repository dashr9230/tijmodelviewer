
#include "main.h"

RsGlobalType& RsGlobal = *(RsGlobalType*)0x004EE9E0;
RwBool& RwInitialized = *(RwBool*)0x004E7138;
RwBool& ForegroundApp = *(RwBool*)0x004DB4B4;

int* KeyTableEnglish = (int*)0x004BC440;

RwBool* validDelta = (RwBool*)0x004E7160;

LRESULT CALLBACK MainWndProc(HWND window,
	UINT message, WPARAM wParam, LPARAM lParam)
{
	POINTS points;
	static BOOL noMemory = FALSE;

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

		case WM_SIZE:
        {
			Log("WM_SIZE");

            RwRect r;

            r.x = 0;
            r.y = 0;
            r.w = LOWORD(lParam);
            r.h = HIWORD(lParam);

#ifdef RWMOUSE
            ClipMouseToWindow(window);
#endif

            if (RwInitialized && r.h > 0 && r.w > 0)
            {
                RsEventHandler(rsCAMERASIZE, &r);

                if (r.w != LOWORD(lParam) && r.h != HIWORD(lParam))
                {
                    WINDOWPLACEMENT     wp;

                    noMemory = TRUE;

                    ReleaseCapture();

                    GetWindowPlacement(window, &wp);
                    if (wp.showCmd == SW_SHOWMAXIMIZED)
                    {
                        SendMessage(window, WM_WINDOWPOSCHANGED, 0, 0);
                    }
                }
                else
                {
                    noMemory = FALSE;
                }

            }

            return 0L;
        }

        case WM_SIZING:
        {
            RECT               *newPos = (LPRECT) lParam;
            RECT                rect;

            if (RwInitialized)
            {
                RsEventHandler(rsIDLE, NULL);
            }

            rect.left = rect.top = 0;
            rect.bottom = newPos->bottom - newPos->top;
            rect.right = newPos->right - newPos->left;

            SetWindowPos(window, HWND_TOP, rect.left, rect.top,
                         (rect.right - rect.left),
                         (rect.bottom - rect.top), SWP_NOMOVE);

            return 0L;
        }

		case WM_KEYDOWN:
		{
			RsKeyStatus ks;

			if (!(lParam & 0x40000000))
			{
				ks.keyScanCode = winTranslateKey(wParam, lParam);
				ks.keyCharCode = KeyTableEnglish[ks.keyScanCode];
				RsKeyboardEventHandler(rsKEYDOWN, &ks);

				if (ks.keyCharCode == rsESC)
				{
					RsEventHandler(rsQUITAPP, NULL);
				}
			}

			return 0L;
		}

		case WM_KEYUP:
		{
			RsKeyStatus ks;

			ks.keyScanCode = winTranslateKey(wParam, lParam);
			ks.keyCharCode = KeyTableEnglish[ks.keyScanCode];

			RsKeyboardEventHandler(rsKEYUP, &ks);

			return 0L;
		}

		case WM_SYSKEYDOWN:
		{
			RsKeyStatus ks;

			if (!(lParam & 0x40000000))
			{
				ks.keyScanCode = winTranslateKey(wParam, lParam);
				ks.keyCharCode = KeyTableEnglish[ks.keyScanCode];

				RsKeyboardEventHandler(rsKEYDOWN, &ks);
			}

			return 0L;
		}

		case WM_SYSKEYUP:
		{
			RsKeyStatus ks;

			ks.keyScanCode = winTranslateKey(wParam, lParam);
			ks.keyCharCode = KeyTableEnglish[ks.keyScanCode];

			RsKeyboardEventHandler(rsKEYUP, &ks);

			return 0L;
		}

		case WM_MOUSEMOVE:
		{
			if (ForegroundApp)
			{
				points = MAKEPOINTS(lParam);

				if (*validDelta)
				{
					RsMouseStatus ms;

					ms.delta.x = points.x - PSGLOBAL(lastMousePos).x;
					ms.delta.y = points.y - PSGLOBAL(lastMousePos).y;
					ms.pos.x = points.x;
					ms.pos.y = points.y;

					RsMouseEventHandler(rsMOUSEMOVE, &ms);
				}
				else
				{
					*validDelta = TRUE;
				}

				PSGLOBAL(lastMousePos).x = points.x;
				PSGLOBAL(lastMousePos).y = points.y;
			}

			return 0L;
		}

		case WM_LBUTTONDOWN:
		{
			RsMouseStatus ms;

			points = MAKEPOINTS(lParam);
			ms.pos.x = points.x;
			ms.pos.y = points.y;
			ms.shift = (wParam & MK_SHIFT) ? TRUE : FALSE;
			ms.control = (wParam & MK_CONTROL) ? TRUE : FALSE;

			SetCapture(window);

			RsMouseEventHandler(rsLEFTBUTTONDOWN, &ms);

			return 0L;
		}

		case WM_LBUTTONUP:
		{
			ReleaseCapture();

			RsMouseEventHandler(rsLEFTBUTTONUP, NULL);

			return 0L;
		}
	}

	return DefWindowProc(window, message, wParam, lParam);
}

INT WINAPI WinMain_HOOK(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	RwV2d pos;
	MSG message;

	Log("WinMain_HOOK start");

	// Change title (16 bytes is the maximum until overwrite other data)
	strncpy((char*)0x004CF8A4, "tijmodelviewer", 16);

	if (RsEventHandler(rsINITIALIZE, NULL) == rsEVENTERROR)
	{
		Log("rsINITIALIZE event failed.");
		return 0;
	}

	Log("Screen size: %dx%d", RsGlobal.maximumWidth, RsGlobal.maximumHeight);

	// Detour game's MainWndProc to ours in InitApplication (0x00431F30)
	*(DWORD*)0x00431F40 = (DWORD)MainWndProc;

	if (!InitApplication(hInstance))
	{
		Log("InitApplication failed.");
		return 0;
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
		Log("InitInstance failed.");
		return 0;
	}

	PSGLOBAL(instance) = hInstance;

	// Detour game's AttachInputDevices to ours, to support also mouse movement
	InstallCallHook(0x00418D67, (DWORD)AttachInputDevices);

	if (rsEVENTERROR == RsEventHandler(rsRWINITIALIZE, PSGLOBAL(window)))
	{
		Log("rsRWINITIALIZE event failed.");

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

	Log("WinMain_HOOK end");

	return message.wParam;
}

LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	PCONTEXT pContextRecord = ExceptionInfo->ContextRecord;

	Log("\nException At Address: 0x%08X\n"
		"EAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\n"
		"ESI: 0x%08X\tEDI: 0x%08X\tEBP: 0x%08X\tESP: 0x%08X\n"
		"EFLAGS: 0x%08X\n",
		pContextRecord->Eip,
		pContextRecord->Eax,
		pContextRecord->Ebx,
		pContextRecord->Ecx,
		pContextRecord->Edx,
		pContextRecord->Esi,
		pContextRecord->Edi,
		pContextRecord->Ebp,
		pContextRecord->Esp,
		pContextRecord->EFlags);

	return EXCEPTION_EXECUTE_HANDLER;
}

void RemovePageProtection()
{
	DWORD dwOldProt;
	if (VirtualProtect((LPVOID)0x00401000, 0x10D37F, PAGE_EXECUTE_READWRITE, &dwOldProt) == FALSE)
	{
		Log("VirtualProtect failed.\n");
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

	if (pData[0] != 0x8A &&
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
		Log("DllMain");

		DisableThreadLibraryCalls(hinstDLL);

		SetUnhandledExceptionFilter(ExceptionFilter);

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
