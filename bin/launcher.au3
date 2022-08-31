
#NoTrayIcon

#include <WinAPI.au3>
#include <Memory.au3>

$sLocalPath = @ScriptDir & '\'

$sStageExe = $sLocalPath & 'stg089.exe'
If Not FileExists($sStageExe) Then
   MsgBox(16, 'Launcher error', 'Executable "stg089.exe" is not found in the local directory.')
   Exit 1
EndIf

$sModule = $sLocalPath & 'tijmodelviewer.dll'
If Not FileExists($sModule) Then
   MsgBox(16, 'Launcher error', 'Module "tijmodelviewer.dll" is not found in the local directory.')
   Exit 2
EndIf

$tSTARTUPINFO = DllStructCreate($tagSTARTUPINFO)
$tPROCESS_INFORMATION = DllStructCreate($tagPROCESS_INFORMATION)

DllStructSetData($tSTARTUPINFO, 'Size', DllStructGetSize($tSTARTUPINFO))

$bResult = _WinAPI_CreateProcess(Null, $sStageExe, Null, Null, False, _
	  $CREATE_SUSPENDED, Null, $sLocalPath, $tSTARTUPINFO, $tPROCESS_INFORMATION)
If Not $bResult Then
	MsgBox(16, 'Launcher error', 'Failed to execute "stg089.exe". Error code: ' & _WinAPI_GetLastError())
	Exit 3
EndIf

$hProcess = DllStructGetData($tPROCESS_INFORMATION, 'hProcess')
$nPathLen = StringLen($sModule) + 1

$pData = _MemVirtualAllocEx($hProcess, Null, $nPathLen, $MEM_COMMIT, $PAGE_READWRITE)

$tBuffer = DllStructCreate('CHAR[' & $nPathLen & ']')
DllStructSetData($tBuffer, 1, $sModule)

$nBytesWritten = 0
$nThreadID = 0
_WinAPI_WriteProcessMemory($hProcess, $pData, DllStructGetPtr($tBuffer), DllStructGetSize($tBuffer), $nBytesWritten)

$pLoadLibraryA = _WinAPI_GetProcAddress(_WinAPI_GetModuleHandle('kernel32'), 'LoadLibraryA')
$hThread = _WinAPI_CreateRemoteThread($hProcess, Null, 0, $pLoadLibraryA, $pData, 0, $nThreadID)
_WinAPI_WaitForSingleObject($hThread, 0xFFFFFFFF)
_WinAPI_CloseHandle($hThread)

_MemVirtualFreeEx($hProcess, $pData, $nPathLen, $MEM_RELEASE)

_WinAPI_ResumeThread(DllStructGetData($tPROCESS_INFORMATION, 'hThread'))

$tBuffer = 0
$tPROCESS_INFORMATION = 0
$tSTARTUPINFO = 0

Func _WinAPI_ResumeThread($hThread)
   Local $aResult = DllCall('kernel32.dll', 'DWORD', 'ResumeThread', 'HANDLE', $hThread)
   If @error Or (_WinAPI_DWordToInt($aResult[0]) = -1) Then Return SetError(1, 0, -1)
   Return $aResult[0]
EndFunc

Func _WinAPI_CreateRemoteThread($hProcess,$tSecurityAttr,$iStackSize,$pStartAddr,$pParameter,$iFlags, ByRef $pThreadID)
   Local $aResult = DllCall('kernel32.dll', 'HANDLE', 'CreateRemoteThread', 'HANDLE', $hProcess, 'struct*', $tSecurityAttr, 'ulong_ptr', $iStackSize, _
	  'ptr', $pStartAddr, 'ptr', $pParameter, 'dword', $iFlags, 'dword*', 0)
   If @error Then Return SetError(@error, @extended, False)
   $pThreadID = $aResult[7]
   Return $aResult[0]
EndFunc
