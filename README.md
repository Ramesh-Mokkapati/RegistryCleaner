# Registry Cleaner

A Windows MFC utility that finds stale registry entries pointing to files that no longer exist, and can
remove them - with a restorable `.reg` backup first.

Split out of DiskForge, where it used to be the built-in Registry Scanner.

## Features

- **15 scan categories**, each selectable with a checkbox:
  - **App Paths** - `HKLM\App Paths` subkeys whose default-value executable is missing
  - **Browser Helper** - `HKLM\...\Browser Helper Objects` CLSIDs whose `InprocServer32` DLL is missing
  - **File Extensions** - `HKCR` extension keys whose registered ProgID no longer exists
  - **Firewall Rules** - `HKLM\...\FirewallRules` values whose `App=` path no longer exists
  - **Fonts** - `HKLM\...\Fonts` values whose font file (relative to `%SystemRoot%\Fonts\` or absolute) is missing
  - **Help Files** - `HKLM\SOFTWARE\Microsoft\Windows\Help` values whose `.chm`/`.hlp` path is missing
  - **Installers** - Uninstall subkeys whose `InstallLocation` folder no longer exists
  - **Interface (COM)** - `HKCR\CLSID` entries with a `LocalServer32` executable that is missing (System32 skipped)
  - **MUI Cache** - `HKCU\...\MuiCache` entries whose executable (after stripping `.FriendlyAppName` /
    `.ApplicationCompany`) is missing
  - **Shared DLLs** - `HKLM\SharedDLLs` values whose DLL no longer exists
  - **Obsolete Software** - `HKCU`/`HKLM` Uninstall subkeys whose `UninstallString` executable is missing
    (MSI/system executables such as `msiexec.exe` are skipped to avoid false positives)
  - **Open With Apps** - `HKCR\Applications\*\shell\open\command` entries whose executable is missing
  - **Run At Startup** - `HKCU`/`HKLM` Run and RunOnce values whose executable no longer exists
  - **Sound Events** - `HKCU\AppEvents\Schemes` entries whose `.wav` file is missing
  - **Windows Services** - `HKLM\SYSTEM\...\Services` ImagePath executables that are missing (handles
    `\SystemRoot\`, `\??\`, `%SystemRoot%\` and bare paths; skips System32)
- The scan runs on a **background thread** with a progress bar and phase label; the window stays responsive
  and the action buttons are disabled while scanning
- Results show category, name/key, problem, hive (HKCU/HKLM) and status (Pending/Fixed/Ignored)
- **Fix Selected** / **Fix All** remove entries after confirmation
- **Backup & Fix** first saves a UTF-16 LE `.reg` file (double-click it to restore), then fixes everything;
  if the backup can't be written nothing is changed
- **Ignore** hides entries for the current session without touching the registry
- Path extraction handles quoted paths (`"C:\...\app.exe" /args`), unquoted paths and environment variables
  (`%SystemRoot%`, ...)
- Fixes are type-aware: value-only issues (startup, shared DLL) delete just the value; subkey issues
  (uninstall, app path) remove the whole orphaned subkey with `RegDeleteTree`
- Runs as Administrator (the manifest requests elevation) so HKLM entries can be cleaned

## Requirements

- **OS:** Windows 10 / Windows 11 x64
- **Runtime:** Microsoft Visual C++ Redistributable 2022 (x64) - https://aka.ms/vs/17/release/vc_redist.x64.exe
- **Build:** Visual Studio 2022 with the "Desktop development with C++" workload and MFC

## How to Build

Open `RegistryCleaner.sln` in Visual Studio 2022, select **Release | x64** and build, or:

```powershell
msbuild RegistryCleaner.sln /p:Configuration=Release /p:Platform=x64
```

The exe is written to `x64\Release\RegistryCleaner.exe`.

## Continuous Integration

`.github/workflows/build.yml` builds Debug and Release x64 on every push and pull request and uploads
`RegistryCleaner.exe` as an artifact. Every successful push to the default branch is released automatically as
`v1.0.<run number>` (pushing a `v*.*.*` tag releases exactly that tag) with `RegistryCleaner-<tag>-x64.zip`
attached, after checking the exe only depends on Windows and VC++ 2015-2022 runtime DLLs.

## Project Structure

| File | Description |
|------|-------------|
| `RegistryCleaner.sln` | Visual Studio 2022 solution |
| `RegistryCleaner/RegistryCleaner.vcxproj` | MSBuild project (x64, v143, MFC as shared DLL, `requireAdministrator`) |
| `RegistryCleaner/RegistryCleaner.cpp/h` | Application entry point; the scanner dialog is the main window |
| `RegistryCleaner/RegistryScanner.cpp/h` | Scanning engine, fixes, and the `.reg` backup writer |
| `RegistryCleaner/RegistryScannerDialog.cpp/h` | Main window; the scan thread posts `WM_RS_PROGRESS` / `WM_RS_COMPLETE` back to it |
| `RegistryCleaner/RegistryCleaner.rc`, `resource.h` | Dialog, icon, strings, version info |
| `RegistryCleaner/RegistryCleaner.manifest` | DPI awareness, Windows 10/11 compatibility, Common Controls 6 |
| `RegistryCleaner/RegistryCleaner.ico` | Application icon |
