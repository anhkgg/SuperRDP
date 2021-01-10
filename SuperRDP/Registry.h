#if !defined(AFX_REGISTRY_H__E0610A5D_7166_4D02_9D7E_11AF7CF8E229__INCLUDED_)
#define AFX_REGISTRY_H__E0610A5D_7166_4D02_9D7E_11AF7CF8E229__INCLUDED_

//
//#include <afx.h>
#include <windows.h>
#include <string>
/////////////////////////////////////////////////////////////////////////////
// CRegistry window

class CRegistry 
{
// Construction
public:
    CRegistry(HKEY hKey=HKEY_LOCAL_MACHINE);

public:
    BOOL SaveKey(LPCTSTR lpFileName);
    BOOL RestoreKey(LPCTSTR lpFileName);
    //BOOL Read(LPCTSTR lpValueName, CString* lpVal);
    BOOL Read(LPCTSTR lpValueName, DWORD* pdwVal);
    BOOL Read(LPCTSTR lpValueName, int* pnVal);
    BOOL Read(LPCTSTR lpValueName, std::string& lpVal);
    BOOL Read(LPCTSTR lpValueName, std::wstring& lpVal);
    BOOL Write(LPCTSTR lpSubKey, LPCTSTR lpVal);
    BOOL WriteExpandSZ(LPCTSTR lpSubKey, LPCTSTR lpVal);
    BOOL Write(LPCTSTR lpSubKey, DWORD dwVal);
    BOOL Write(LPCTSTR lpSubKey, int nVal);
    BOOL DeleteKey(HKEY hKey, LPCTSTR lpSubKey);
    BOOL DeleteValue(LPCTSTR lpValueName);
    void Close();
    BOOL Open(LPCTSTR lpSubKey, DWORD Flag=0);
    BOOL Open(HKEY root, LPCTSTR lpSubKey, DWORD Flag = 0);
    BOOL CreateKey(LPCTSTR lpSubKey, DWORD Flag = 0);
    BOOL CreateKey(HKEY root, LPCTSTR lpSubKey, DWORD Flag = 0);
    virtual ~CRegistry();

protected:
    HKEY m_hKey;
   
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_REGISTRY_H__E0610A5D_7166_4D02_9D7E_11AF7CF8E229__INCLUDED_)