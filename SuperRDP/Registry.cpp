// Registry.cpp : implementation file
//

#include "pch.h"
#include "Registry.h"
#include <assert.h>
#include <tchar.h>
#pragma comment(lib, "advapi32.lib")
/////////////////////////////////////////////////////////////////////////////
// CRegistry

CRegistry::CRegistry(HKEY hKey)
{
    m_hKey=hKey;
}

CRegistry::~CRegistry()
{
    Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegistry Functions

BOOL CRegistry::CreateKey(LPCTSTR lpSubKey, DWORD Flag/* = 0*/)
{
    assert(m_hKey);
    assert(lpSubKey);

    HKEY hKey;
    DWORD dw;
    long lReturn=RegCreateKeyEx(m_hKey,lpSubKey,0L,NULL,REG_OPTION_VOLATILE,/*KEY_ALL_ACCESS|*/Flag,NULL,&hKey,&dw);
   
    if(lReturn==ERROR_SUCCESS)
    {
        m_hKey=hKey;
        return TRUE;
    }
   
    return FALSE;
   
}

BOOL CRegistry::CreateKey(HKEY root, LPCTSTR lpSubKey, DWORD Flag/* = 0*/)
{
    assert(root);
    assert(lpSubKey);

    m_hKey = root;
    return CreateKey(lpSubKey, Flag);
}

BOOL CRegistry::Open(LPCTSTR lpSubKey, DWORD Flag /*= 0*/)
{
    assert(m_hKey);
    assert(lpSubKey);
   
    HKEY hKey;
    long lReturn=RegOpenKeyEx(m_hKey,lpSubKey,0L,/*KEY_ALL_ACCESS | Flag*/Flag,&hKey);
   
    if(lReturn==ERROR_SUCCESS)
    {
        m_hKey=hKey;
        return TRUE;
    }
    return FALSE;
   
}

BOOL CRegistry::Open(HKEY root, LPCTSTR lpSubKey, DWORD Flag)
{
    assert(root);
    assert(lpSubKey);
    m_hKey = root;

    return Open(lpSubKey, Flag);
}

void CRegistry::Close()
{
    if(m_hKey)
    {
        RegCloseKey(m_hKey);
        m_hKey=NULL;
    }
   
}

BOOL CRegistry::DeleteValue(LPCTSTR lpValueName)
{
    assert(m_hKey);
    assert(lpValueName);
   
    long lReturn=RegDeleteValue(m_hKey,lpValueName);
   
    if(lReturn==ERROR_SUCCESS)
        return TRUE;
    return FALSE;
   
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
    assert(hKey);
    assert(lpSubKey);
   
    long lReturn=RegDeleteValue(hKey,lpSubKey);
   
    if(lReturn==ERROR_SUCCESS)
        return TRUE;
    return FALSE;
   
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, int nVal)
{
    assert(m_hKey);
    assert(lpSubKey);
   
    DWORD dwValue;
    dwValue=(DWORD)nVal;
   
    long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwValue,sizeof(DWORD));
   
       if(lReturn==ERROR_SUCCESS)
        return TRUE;
   
    return FALSE;
   
}

BOOL CRegistry::Write(LPCTSTR lpSubKey, DWORD dwVal)
{
    assert(m_hKey);
    assert(lpSubKey);
   
    long lReturn=RegSetValueEx(m_hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVal,sizeof(DWORD));
   
       if(lReturn==ERROR_SUCCESS)
        return TRUE;
   
    return FALSE;
   
}

BOOL CRegistry::Write(LPCTSTR lpValueName, LPCTSTR lpValue)
{
    assert(m_hKey);
    assert(lpValueName);
    assert(lpValue);  

    long lReturn=RegSetValueEx(m_hKey,lpValueName,0L,REG_SZ,(const BYTE *) lpValue, _tcslen(lpValue)*sizeof(TCHAR)+sizeof(TCHAR));
   
       if(lReturn==ERROR_SUCCESS)
        return TRUE;
   
    return FALSE;
   
}

BOOL CRegistry::WriteExpandSZ(LPCTSTR lpValueName, LPCTSTR lpValue)
{
    assert(m_hKey);
    assert(lpValueName);
    assert(lpValue);

    long lReturn = RegSetValueEx(m_hKey, lpValueName, 0L, REG_EXPAND_SZ, (const BYTE *)lpValue, _tcslen(lpValue) * sizeof(TCHAR) + sizeof(TCHAR));

    if (lReturn == ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpValueName, int* pnVal)
{
    assert(m_hKey);
    assert(lpValueName);
    assert(pnVal);
   
    DWORD dwType;
    DWORD dwSize=sizeof(DWORD);
    DWORD dwDest;
    long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
   
    if(lReturn==ERROR_SUCCESS)
    {
        *pnVal=(int)dwDest;
        return TRUE;
    }
    return FALSE;
   
}

BOOL CRegistry::Read(LPCTSTR lpValueName, DWORD* pdwVal)
{
    assert(m_hKey);
    assert(lpValueName);
    assert(pdwVal);
   
    DWORD dwType;
    DWORD dwSize=sizeof(DWORD);
    DWORD dwDest;
    long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
   
    if(lReturn==ERROR_SUCCESS)
    {
        *pdwVal=dwDest;
        return TRUE;
    }
    return FALSE;
   
}

BOOL CRegistry::RestoreKey(LPCTSTR lpFileName)
{
    assert(m_hKey);
    assert(lpFileName);
   
    long lReturn=RegRestoreKey(m_hKey,lpFileName,REG_WHOLE_HIVE_VOLATILE);
   
    if(lReturn==ERROR_SUCCESS)
        return TRUE;
   
    return FALSE;
}

BOOL CRegistry::SaveKey(LPCTSTR lpFileName)
{
    assert(m_hKey);
    assert(lpFileName);
   
    long lReturn=RegSaveKey(m_hKey,lpFileName,NULL);
   
    if(lReturn==ERROR_SUCCESS)
        return TRUE;
   
    return FALSE;
}

//BOOL CRegistry::Read(LPCTSTR lpValueName, CString* lpVal)
//{
//    assert(m_hKey);
//    assert(lpValueName);
//    assert(lpVal);
//   
//    DWORD dwType;
//    DWORD dwSize=200;
//    char szString[2550];
//   
//    long lReturn=RegQueryValueEx(m_hKey,lpValueName,NULL,&dwType,(BYTE *)szString,&dwSize);
//   
//    if(lReturn==ERROR_SUCCESS)
//    {
//        *lpVal=szString;
//        return TRUE;
//    }
//    return FALSE;
//   
//}

BOOL CRegistry::Read(LPCTSTR lpValueName, std::string& lpVal)
{
    assert(m_hKey);
    assert(lpValueName);

    DWORD dwType;
    DWORD dwSize = 200;
    char szString[2550];

    long lReturn = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *)szString, &dwSize);

    if (lReturn == ERROR_SUCCESS)
    {
        //lpVal = szString;
        std::string str(reinterpret_cast<const char*>(szString), dwSize);
        lpVal = str;

        return TRUE;
    }
    return FALSE;

}

BOOL CRegistry::Read(LPCTSTR lpValueName, std::wstring& lpVal)
{
    assert(m_hKey);
    assert(lpValueName);

    DWORD dwType;
    DWORD dwSize = 200;
    char szString[2550];

    long lReturn = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *)szString, &dwSize);

    if (lReturn == ERROR_SUCCESS)
    {
        //lpVal = szString;
        std::wstring str(reinterpret_cast<const wchar_t*>(szString), dwSize);
        lpVal = str;

        return TRUE;
    }
    return FALSE;

}