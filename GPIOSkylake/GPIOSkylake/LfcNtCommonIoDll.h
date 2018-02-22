#ifndef WINIO_H
#define WINIO_H

#include "LfcNtCommonIo.h"

#ifndef WINIO_DLL
#define WINIO_API _declspec(dllimport)
#else
#define WINIO_API 
#endif

extern "C"
{
	WINIO_API bool _stdcall InitializeWinIo();
	WINIO_API void _stdcall ShutdownWinIo();
	WINIO_API PBYTE _stdcall MapPhysToLin(tagPhysStruct &PhysStruct);
	WINIO_API bool _stdcall UnmapPhysicalMemory(tagPhysStruct &PhysStruct);
	WINIO_API bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
	WINIO_API bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
	WINIO_API bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
	WINIO_API bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
	WINIO_API bool _stdcall InstallWinIoDriver(PWSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
	WINIO_API bool _stdcall RemoveWinIoDriver();
	WINIO_API bool _stdcall WinIoLinearToPhysical(PBYTE LinearAddress, LARGE_INTEGER *PhysicalAddress);
	WINIO_API bool _stdcall WinIoTriggerSmi(struct RegisterTable *RegTable);
	WINIO_API bool _stdcall EcAndKbcFunction(PEC_EXCHANGE_BUFFER EcExchangeBufferInOut);
	WINIO_API bool _stdcall EcRamRw(PEC_RAM_RW_BUFFER EcRamRwBuffer);
	WINIO_API bool _stdcall EcDiagRamRw(PEC_DIAG_RAM_RW_BUFFER EcDiagRamRwBuffer);
	WINIO_API bool _stdcall PciConfigFunction(PPCI_EXCHANGE_BUFFER PciExchangeBufferInOut);
	WINIO_API bool _stdcall CmosFunction(PCMOS_EXCHANGE_BUFFER CmosExchangeBufferInOut);
	WINIO_API bool _stdcall MsrFunction(PMSR_EXCHANGE_BUFFER MsrExchangeBufferInOut);
	WINIO_API bool _stdcall EcSmbusFunction(PEC_SMBUS_EXCHANGE_BUFFER EcSmbusExchangeBufferInOut);
}

extern HANDLE hDriver;
extern bool IsWinIoInitialized;
extern bool g_Is64BitOS;

bool _stdcall StartWinIoDriver();
bool _stdcall StopWinIoDriver();

#endif
