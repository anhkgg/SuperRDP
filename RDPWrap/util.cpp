#include "stdafx.h"
#include "IniFile.h"
#include <stdlib.h>

wchar_t LogFile[256] = L"c:\\rdpwrap.txt\0";



#ifdef LOG

#include <strsafe.h>
void WriteToLog(const char* szfmt, ...)
{
    char szBuf[4096] = { 0 };
    va_list arg;

    va_start(arg, szfmt);
    StringCbVPrintfA(szBuf, 4096, szfmt, arg);
    va_end(arg);

    WriteToLog(szBuf);
}

void WriteToLog(LPSTR Text)
{
    OutputDebugStringA(Text);
    DWORD dwBytesOfWritten;
    HANDLE hFile = CreateFile(LogFile, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return;
    SetFilePointer(hFile, 0, 0, FILE_END);
    WriteFile(hFile, Text, strlen(Text), &dwBytesOfWritten, NULL);
    CloseHandle(hFile);
}
#else
#define  WriteToLog(x) ((void)0)
#endif 

DWORD INIReadDWordHex(INI_FILE *IniFile, char *Sect, char *VariableName, PLATFORM_DWORD Default)
{
    INI_VAR_DWORD Variable;

    if (IniFile->GetVariableInSection(Sect, VariableName, &Variable))
    {
        return Variable.ValueHex;
    }
    return Default;
}

void INIReadString(INI_FILE *IniFile, char *Sect, char *VariableName, char *Default, char *Ret, DWORD RetSize)
{
    INI_VAR_STRING Variable;

    memset(Ret, 0x00, RetSize);
    if (!IniFile->GetVariableInSection(Sect, VariableName, &Variable))
    {
        strcpy_s(Ret, RetSize, Default);
        return;
    }
    strcpy_s(Ret, RetSize, Variable.Value);
}

HMODULE GetCurrentModule()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)GetCurrentModule, &hModule);
    return hModule;
}

/*PLATFORM_DWORD SearchAddressBySignature(char *StartPosition, PLATFORM_DWORD Size, char *Signature, int SignatureSize)
{
    PLATFORM_DWORD AddressReturn = -1;

    for (PLATFORM_DWORD i = 0; i < Size; i++)
    {
        for (int j = 0; StartPosition[i+j] == Signature[j] && j < SignatureSize; j++)
        {
            if (j == SignatureSize-1) AddressReturn = (PLATFORM_DWORD)&StartPosition[i];
        }
    }

    return AddressReturn;
}*/

bool GetModuleCodeSectionInfo(HMODULE hModule, PLATFORM_DWORD *BaseAddr, PLATFORM_DWORD *BaseSize)
{
    PIMAGE_DOS_HEADER		pDosHeader;
    PIMAGE_FILE_HEADER      pFileHeader;
    PIMAGE_OPTIONAL_HEADER  pOptionalHeader;

    if (hModule == NULL) return false;

    pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    pFileHeader = (PIMAGE_FILE_HEADER)(((PBYTE)hModule) + pDosHeader->e_lfanew + 4);
    pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)(pFileHeader + 1);

    *BaseAddr = (PLATFORM_DWORD)hModule;
    *BaseSize = (PLATFORM_DWORD)pOptionalHeader->SizeOfCode;

    if (*BaseAddr <= 0 || *BaseSize <= 0) return false;
    return true;
}

void SetThreadsState(bool Resume)
{
    HANDLE h, hThread;
    DWORD CurrTh, CurrPr;
    THREADENTRY32 Thread;

    CurrTh = GetCurrentThreadId();
    CurrPr = GetCurrentProcessId();

    h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (h != INVALID_HANDLE_VALUE)
    {
        Thread.dwSize = sizeof(THREADENTRY32);
        Thread32First(h, &Thread);
        do
        {
            if (Thread.th32ThreadID != CurrTh && Thread.th32OwnerProcessID == CurrPr)
            {
                hThread = OpenThread(THREAD_SUSPEND_RESUME, false, Thread.th32ThreadID);
                if (hThread != INVALID_HANDLE_VALUE)
                {
                    if (Resume)		ResumeThread(hThread);
                    else			SuspendThread(hThread);
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(h, &Thread));
        CloseHandle(h);
    }
}

BOOL __stdcall GetModuleVersion(LPCWSTR lptstrModuleName, FILE_VERSION *FileVersion)
{
    typedef struct
    {
        WORD             wLength;
        WORD             wValueLength;
        WORD             wType;
        WCHAR            szKey[16];
        WORD             Padding1;
        VS_FIXEDFILEINFO Value;
        WORD             Padding2;
        WORD             Children;
    } VS_VERSIONINFO;

    HMODULE hMod = GetModuleHandle(lptstrModuleName);
    if (!hMod)
    {
        return false;
    }

    HRSRC hResourceInfo = FindResourceW(hMod, (LPCWSTR)1, (LPCWSTR)0x10);
    if (!hResourceInfo)
    {
        return false;
    }

    VS_VERSIONINFO *VersionInfo = (VS_VERSIONINFO*)LoadResource(hMod, hResourceInfo);
    if (!VersionInfo)
    {
        return false;
    }

    FileVersion->dwVersion = VersionInfo->Value.dwFileVersionMS;
    FileVersion->Release = (WORD)(VersionInfo->Value.dwFileVersionLS >> 16);
    FileVersion->Build = (WORD)VersionInfo->Value.dwFileVersionLS;

    return true;
}

BOOL __stdcall GetFileVersion(LPCWSTR lptstrFilename, FILE_VERSION *FileVersion)
{
    typedef struct
    {
        WORD             wLength;
        WORD             wValueLength;
        WORD             wType;
        WCHAR            szKey[16];
        WORD             Padding1;
        VS_FIXEDFILEINFO Value;
        WORD             Padding2;
        WORD             Children;
    } VS_VERSIONINFO;

    HMODULE hFile = LoadLibraryExW(lptstrFilename, NULL, LOAD_LIBRARY_AS_DATAFILE);
    if (!hFile)
    {
        return false;
    }

    HRSRC hResourceInfo = FindResourceW(hFile, (LPCWSTR)1, (LPCWSTR)0x10);
    if (!hResourceInfo)
    {
        return false;
    }

    VS_VERSIONINFO *VersionInfo = (VS_VERSIONINFO*)LoadResource(hFile, hResourceInfo);
    if (!VersionInfo)
    {
        return false;
    }

    FileVersion->dwVersion = VersionInfo->Value.dwFileVersionMS;
    FileVersion->Release = (WORD)(VersionInfo->Value.dwFileVersionLS >> 16);
    FileVersion->Build = (WORD)VersionInfo->Value.dwFileVersionLS;

    return true;
}