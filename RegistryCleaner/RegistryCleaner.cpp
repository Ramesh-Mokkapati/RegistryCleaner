// RegistryCleaner.cpp - Application entry point. The Registry Scanner dialog
// is the main window.
#include "pch.h"
#include "RegistryCleaner.h"
#include "RegistryScannerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRegistryCleanerApp theApp;

BOOL CRegistryCleanerApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("DiskForge"));

    CRegistryScannerDialog dlg;
    m_pMainWnd = &dlg;    dlg.DoModal();
    return FALSE; // dialog closed - exit instead of starting the message pump
}
