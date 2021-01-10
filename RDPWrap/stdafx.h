

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN            
#define _CRT_SECURE_NO_WARNINGS
#define WINDOWS_IGNORE_PACKING_MISMATCH

#include <windows.h>
#include <TlHelp32.h>

#include "IniFile.h"

#define LOG

typedef VOID	(WINAPI* SERVICEMAIN)(DWORD, LPTSTR*);
typedef VOID	(WINAPI* SVCHOSTPUSHSERVICEGLOBALS)(VOID*);
typedef HRESULT (WINAPI* SLGETWINDOWSINFORMATIONDWORD)(PCWSTR, DWORD*);


typedef struct
{
    union
    {
        struct
        {
            WORD Minor;
            WORD Major;
        } wVersion;
        DWORD dwVersion;
    };
    WORD Release;
    WORD Build;
} FILE_VERSION;

#ifdef _WIN64
typedef unsigned long long PLATFORM_DWORD;
#pragma pack(push, 1)
struct FARJMP
{	// x64 far jump | opcode | assembly
    BYTE MovOp;		// 48	mov rax, ptr
    BYTE MovRegArg;	// B8
    DWORD64 MovArg;	// PTR
    BYTE PushRaxOp; // 50	push rax
    BYTE RetOp;		// C3	retn
};//1+1+8+1+1=12
#pragma pack(pop)
/*
mov rax, ptr //48 b8 ptr
push rax;//50
retn;//c3
*/
#else
typedef unsigned long PLATFORM_DWORD;
#pragma pack(push, 1)
struct FARJMP
{	// x86 far jump | opcode | assembly
    BYTE PushOp;	// 68	push ptr
    DWORD PushArg;	// PTR
    BYTE RetOp;		// C3	retn
};
#pragma pack(pop)
/*
push ptr;//68 ptr
retn;//c3
*/
#endif

void WriteToLog(const char* szfmt, ...);
void WriteToLog(LPSTR Text);

DWORD INIReadDWordHex(INI_FILE *IniFile, char *Sect, char *VariableName, PLATFORM_DWORD Default);
void INIReadString(INI_FILE *IniFile, char *Sect, char *VariableName, char *Default, char *Ret, DWORD RetSize);
HMODULE GetCurrentModule();
bool GetModuleCodeSectionInfo(HMODULE hModule, PLATFORM_DWORD *BaseAddr, PLATFORM_DWORD *BaseSize);
void SetThreadsState(bool Resume);
BOOL __stdcall GetModuleVersion(LPCWSTR lptstrModuleName, FILE_VERSION *FileVersion);
BOOL __stdcall GetFileVersion(LPCWSTR lptstrFilename, FILE_VERSION *FileVersion);