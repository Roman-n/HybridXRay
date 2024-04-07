#include "stdafx.h"
#pragma hdrstop

#include <commdlg.h>
#include <objbase.h>
#include <cderr.h>
#include <shobjidl.h>
#include "FileDialog.h"

typedef HRESULT(*LPFNSHCREATEITEMFROMPARSINGNAMEPROC)(PCWSTR path, void*, REFIID riid, void** ppv);
static LPFNSHCREATEITEMFROMPARSINGNAMEPROC pSHCreateItemFromParsingName;

#ifdef M_BORLAND
// interface IDs
static const GUID IID_IShellItem       = {0x43826d1e, 0xe718, 0x42ee, {0xbc, 0x55, 0xa1, 0xe2, 0x61, 0xc3, 0x7b, 0xfe}};
static const GUID IID_IShellItemFilter = {0x2659b475, 0xeeb8, 0x48b7, {0x8f, 0x07, 0xb3, 0x78, 0x81, 0x0f, 0x48, 0xcf}};
static const GUID IID_IShellItemArray  = {0xb63ea76d, 0x1f85, 0x456f, {0xa1, 0x9c, 0x48, 0x15, 0x9e, 0xfa, 0x85, 0x8b}};
static const GUID IID_IModalWindow     = {0xb4db1657, 0x70d7, 0x485e, {0x8e, 0x3e, 0x6f, 0xcb, 0x5a, 0x5c, 0x18, 0x02}};
static const GUID IID_IFileDialog      = {0x42f85136, 0xdb7e, 0x439c, {0x85, 0xf1, 0xe4, 0x07, 0x5d, 0x13, 0x5f, 0xc8}};
static const GUID IID_IFileOpenDialog  = {0xd57c7288, 0xd4ad, 0x4768, {0xbe, 0x02, 0x9d, 0x96, 0x95, 0x32, 0xd9, 0x60}};
static const GUID IID_IFileSaveDialog  = {0x84bccd23, 0x5fde, 0x4cdb, {0xae, 0xa4, 0xaf, 0x64, 0xb8, 0x3d, 0x78, 0xab}};

// CLSIDs
static const GUID CLSID_ShellItem      = {0x9ac9fbe1, 0xe0a2, 0x4ad6, {0xb4, 0xee, 0xe2, 0x12, 0x01, 0x3e, 0xa9, 0x17}};
static const GUID CLSID_FileOpenDialog = {0xdc1c5a9c, 0xe88a, 0x4dde, {0xa5, 0xa1, 0x60, 0xf8, 0x2a, 0x20, 0xae, 0xf7}};
static const GUID CLSID_FileSaveDialog = {0xc0b4e2f3, 0xba21, 0x4773, {0x8d, 0xba, 0x33, 0x5e, 0xc9, 0x46, 0xeb, 0x8b}};
#endif   // M_BORLAND

static std::wstring ansi2wide(xr_string str)
{
    std::wstring ret;
    size_t       sz = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
    ret.resize(sz);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &*ret.begin(), ret.size());
    return ret;
}

static xr_string wide2ansi(std::wstring str)
{
    xr_string ret;
    size_t    sz = WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.length(), NULL, 0, NULL, NULL);
    ret.resize(sz);
    WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.length(), &*ret.begin(), ret.length(), NULL, NULL);
    return ret;
}

static xr_string tofullpath(xr_string path)
{
    if (path.length() >= 2 && (path[1] == ':' || (path[0] == '\\' && path[1] == '\\')))
        return path;

    char cd[512];
    GetCurrentDirectory(sizeof(cd), cd);

    xr_string current_path = cd;
    char      last         = current_path[current_path.length() - 1];
    if (last != '\\' && last != '/')
        current_path += '\\';

    return current_path + path;
}

CFileDialog::CFileDialog(int dialog_type)
{
    Multiselect     = false;
    ShowReadOnly    = false;
    FileMustExist   = false;
    PathMustExist   = false;
    OverwritePrompt = false;
    NoChangeDir     = false;
    NoPlacesBar     = false;

    if (dialog_type == fdOpen)
        Caption = "Open";
    if (dialog_type == fdSave)
        Caption = "Save";

    DefaultExt    = "";
    DefaultDir    = "";
    CurrentFile   = "";
    CurrentDir    = "";

    Type          = dialog_type;
    DefaultFilter = 0;
    CurrentFilter = -1;
}

void CFileDialog::SetFilters(const char* filters)
{
    Filters.clear();

    const char* p = filters;
    while (p[0])
    {
        const char* desc = p;
        p                = strchr(desc, 0) + 1;
        const char* mask = p;
        p                = strchr(mask, 0) + 1;

        AddFilter(mask, desc);
    }
}

bool CFileDialog::ShowModal(HWND owner)
{
    Results.clear();

    if (DefaultExt.length())
    {
        if (DefaultExt.length() >= 2 && DefaultExt[0] == '*' && DefaultExt[1] == '.')
            DefaultExt.erase(0, 2);
        else if (DefaultExt.length() >= 1 && DefaultExt[0] == '.')
            DefaultExt.erase(0, 1);
    }

    int ret;
    if ((GetVersion() & 0xFF) >= 6)   // if NT6 or greater
    {
        ret = ShowVista(owner);
        if (ret < 0)
        {
            Log("Vista dialog failed :-(");
            ret = ShowOld(owner);
        }
    }
    else
        ret = ShowOld(owner);

    return ret > 0;
}

int CFileDialog::ShowOld(HWND owner)
{
    char result[512 * 256];
    strcpy(result, CurrentFile.c_str());
    OPENFILENAME ofn    = {0};

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = owner;
    ofn.lpstrFile       = result;
    ofn.nMaxFile        = sizeof(result);
    ofn.lpstrInitialDir = DefaultDir.size() ? DefaultDir.c_str() : NULL;
    ofn.lpstrTitle      = Caption.size() ? Caption.c_str() : NULL;

    DWORD flags         = 0;

    if (Multiselect)
        flags |= OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    if (!ShowReadOnly)
        flags |= OFN_HIDEREADONLY;
    if (PathMustExist)
        flags |= OFN_PATHMUSTEXIST;
    if (FileMustExist)
        flags |= OFN_FILEMUSTEXIST;
    if (OverwritePrompt)
        flags |= OFN_OVERWRITEPROMPT;
    if (NoChangeDir)
        flags |= OFN_NOCHANGEDIR;

    ofn.Flags       = flags;
    ofn.lpstrDefExt = DefaultExt.size() ? DefaultExt.c_str() : NULL;

    if (NoPlacesBar)
        ofn.FlagsEx = OFN_EX_NOPLACESBAR;

    char flt[1024] = "";
    if (Filters.size())
    {
        char* p = flt;
        for (size_t i = 0; i < Filters.size(); i++)
        {
            SFilter& f = Filters[i];
            size_t   len;

            len = f.desc.length();
            memcpy(p, f.desc.c_str(), len + 1);
            p += len + 1;
            len = f.mask.length();
            memcpy(p, f.mask.c_str(), len + 1);
            p += len + 1;
        }
        *p              = '\0';
        ofn.lpstrFilter = flt;
    }

    if (CurrentFilter != -1)
        ofn.nFilterIndex = CurrentFilter + 1;
    else
        ofn.nFilterIndex = DefaultFilter + 1;

    if (Type == fdOpen)
    {
        if (GetOpenFileName(&ofn) == FALSE)
        {
            if (CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
                SetLastError(DWORD(ERROR_INSUFFICIENT_BUFFER));
            return 0;
        }
    }
    else
    {
        if (GetSaveFileName(&ofn) == FALSE)
        {
            if (CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
                SetLastError(DWORD(ERROR_INSUFFICIENT_BUFFER));
            return 0;
        }
    }
    // if multiselect and selected more than one file
    if (Type == fdOpen && Multiselect && result[strlen(result) + 1] != '\0')
    {
        char* p = result;

        char  dir[512];
        strcpy(dir, p);

        p = strchr(p, 0) + 1;
        while (p[0])
        {
            Results.push_back(xr_string(dir) + "\\" + p);
            p = strchr(p, 0) + 1;
        }
    }
    else
        Results.push_back(result);

    CurrentFilter = ofn.nFilterIndex - 1;
    CurrentFile   = Results[0];
    char drive[_MAX_DRIVE], dir[_MAX_DIR];
    _splitpath(Results[0].c_str(), drive, dir, NULL, NULL);
    CurrentDir = xr_string(drive) + dir;

    return 1;
}

int CFileDialog::ShowVista(HWND owner)
{
    if (DefaultDir.length())
        DefaultDir = tofullpath(DefaultDir);
    if (CurrentDir.length())
        CurrentDir = tofullpath(CurrentDir);

    if (pSHCreateItemFromParsingName == NULL)
    {
        HMODULE hShell32 = GetModuleHandle("SHELL32.DLL");
        R_ASSERT(hShell32);
        pSHCreateItemFromParsingName = (LPFNSHCREATEITEMFROMPARSINGNAMEPROC)GetProcAddress(hShell32, "SHCreateItemFromParsingName");
        R_ASSERT(pSHCreateItemFromParsingName);
    }

    IFileDialog*     dlg;
    IFileOpenDialog* odlg = NULL;
    IFileSaveDialog* sdlg = NULL;

    int              ret  = -1;

    do
    {
        HRESULT hr;

        if (Type == fdOpen)
        {
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)&odlg);
            if (FAILED(hr))
                break;

            dlg = odlg;   // it's okay??
        }
        else
        {
            hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileSaveDialog, (void**)&sdlg);
            if (FAILED(hr))
                break;

            dlg = sdlg;
        }

        DWORD flags = 0;

        if (Multiselect)
            flags |= FOS_ALLOWMULTISELECT;
        if (PathMustExist)
            flags |= FOS_PATHMUSTEXIST;
        if (FileMustExist)
            flags |= FOS_FILEMUSTEXIST;
        if (NoChangeDir)
            flags |= FOS_NOCHANGEDIR;
        if (OverwritePrompt)
            flags |= FOS_OVERWRITEPROMPT;

        hr = dlg->SetOptions(flags);
        if (FAILED(hr))
            break;

        hr = dlg->SetTitle(ansi2wide(Caption).c_str());
        if (FAILED(hr))
            break;

        IShellItem* si_defdir;
        hr = pSHCreateItemFromParsingName(ansi2wide(DefaultDir).c_str(),
            NULL, IID_IShellItem, (void**)&si_defdir);
        if (!FAILED(hr))
        {
            hr = dlg->SetDefaultFolder(si_defdir);
            si_defdir->Release();
            if (FAILED(hr))
                break;
        }

        if (CurrentDir.size())
        {
            IShellItem* si_cd;
            hr = pSHCreateItemFromParsingName(ansi2wide(CurrentDir).c_str(),
                NULL, IID_IShellItem, (void**)&si_cd);
            if (!FAILED(hr))
            {
                hr = dlg->SetFolder(si_cd);
                si_cd->Release();
                if (FAILED(hr))
                    break;
            }
        }

        hr = dlg->SetDefaultExtension(ansi2wide(DefaultExt).c_str());
        if (FAILED(hr))
            break;

        if (Filters.size())
        {
            xr_vector<COMDLG_FILTERSPEC> specs;
            xr_vector<std::wstring>      names, masks;

            size_t                       filter_cnt = Filters.size();
            specs.resize(filter_cnt);
            names.resize(filter_cnt);
            masks.resize(filter_cnt);

            for (size_t i = 0; i < filter_cnt; i++)
            {
                names[i]         = ansi2wide(Filters[i].desc);
                masks[i]         = ansi2wide(Filters[i].mask);

                specs[i].pszName = (LPCWSTR)names[i].c_str();
                specs[i].pszSpec = (LPCWSTR)masks[i].c_str();
            }

            hr = dlg->SetFileTypes(filter_cnt, &specs.front());
            if (FAILED(hr))
                break;
        }

        if (CurrentFilter != -1)
            /*hr =*/dlg->SetFileTypeIndex(CurrentFilter + 1);
        else
            /*hr =*/dlg->SetFileTypeIndex(DefaultFilter + 1);

        /*hr =*/dlg->SetFileName(ansi2wide(CurrentFile).c_str());

        if (dlg->Show(owner) != S_OK)
        {
            ret = 0;
            break;
        }

        if (Type == fdOpen && Multiselect)
        {
            IShellItemArray* arr;
            hr = odlg->GetResults(&arr);
            if (FAILED(hr))
                break;

            ULONG count;
            hr = arr->GetCount(&count);
            if (FAILED(hr))
            {
                arr->Release();
                break;
            }

            for (ULONG i = 0; i < count; i++)
            {
                IShellItem* si;
                hr = arr->GetItemAt(i, &si);
                if (FAILED(hr))
                {
                    arr->Release();
                    goto fail;
                }

                WCHAR* pfn;
                hr = si->GetDisplayName(SIGDN_FILESYSPATH, &pfn);
                if (FAILED(hr))
                {
                    arr->Release();
                    si->Release();
                    goto fail;
                }

                Results.push_back(wide2ansi((wchar_t*)pfn));
                CoTaskMemFree(pfn);

                si->Release();
            }

            arr->Release();
        }
        else
        {
            IShellItem* si_result;
            hr = dlg->GetResult(&si_result);
            if (FAILED(hr))
                break;

            WCHAR* pfn;
            hr = si_result->GetDisplayName(SIGDN_FILESYSPATH, &pfn);
            si_result->Release();
            if (FAILED(hr))
                break;

            Results.push_back(wide2ansi((wchar_t*)pfn));
            CoTaskMemFree(pfn);
        }

        hr = dlg->GetFileTypeIndex((UINT*)&CurrentFilter);
        if (FAILED(hr))
            CurrentFilter = -1;

        WCHAR* wcfn;
        hr = dlg->GetFileName(&wcfn);
        if (!FAILED(hr))
        {
            CurrentFile = wide2ansi((wchar_t*)wcfn);
            CoTaskMemFree(wcfn);
        }

        IShellItem* si_cd;
        hr = dlg->GetFolder(&si_cd);
        if (!FAILED(hr))
        {
            WCHAR* wcd;
            hr = si_cd->GetDisplayName(SIGDN_FILESYSPATH, &wcd);
            if (!FAILED(hr))
            {
                CurrentDir = wide2ansi((wchar_t*)wcd);
                CoTaskMemFree(wcd);
            }
            si_cd->Release();
        }

        ret = 1;
    }
    while (0);
fail:

    if (odlg)
        odlg->Release();
    if (sdlg)
        sdlg->Release();

    return ret;
}
