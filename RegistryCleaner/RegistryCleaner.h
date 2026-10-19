// RegistryCleaner.h - Stand-alone registry cleaning utility (Registry
// Scanner), split out of DiskForge so the disk tool only touches disks.
#pragma once

class CRegistryCleanerApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
};

extern CRegistryCleanerApp theApp;
