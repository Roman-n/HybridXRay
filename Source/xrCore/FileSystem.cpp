//----------------------------------------------------
// file: FileSystem.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "cderr.h"
#include "commdlg.h"
#include "vfw.h"
#include <shobjidl.h>
#include "FileDialog.h"

EFS_Utils* xr_EFS = NULL;
//----------------------------------------------------
EFS_Utils::EFS_Utils() {}

EFS_Utils::~EFS_Utils() {}

xr_string EFS_Utils::ExtractFileName(LPCSTR src)
{
    string_path name;
    _splitpath(src, 0, 0, name, 0);
    return xr_string(name);
}

xr_string EFS_Utils::ExtractFileExt(LPCSTR src)
{
    string_path ext;
    _splitpath(src, 0, 0, 0, ext);
    return xr_string(ext);
}

xr_string EFS_Utils::ExtractFilePath(LPCSTR src)
{
    string_path drive, dir;
    _splitpath(src, drive, dir, 0, 0);
    return xr_string(drive) + dir;
}

xr_string EFS_Utils::ExcludeBasePath(LPCSTR full_path, LPCSTR excl_path)
{
    LPCSTR sub = strstr(full_path, excl_path);
    if (0 != sub)
        return xr_string(sub + xr_strlen(excl_path));
    else
        return xr_string(full_path);
}

xr_string EFS_Utils::ChangeFileExt(LPCSTR src, LPCSTR ext)
{
    xr_string tmp;
    LPSTR     src_ext = strext(src);
    if (src_ext)
    {
        size_t ext_pos = src_ext - src;
        tmp.assign(src, 0, ext_pos);
    }
    else
    {
        tmp = src;
    }
    tmp += ext;
    return tmp;
}

xr_string EFS_Utils::ChangeFileExt(const xr_string& src, LPCSTR ext)
{
    return ChangeFileExt(src.c_str(), ext);
}

//----------------------------------------------------
int MakeFilter(string1024& dest, LPCSTR info, LPCSTR ext)
{
    std::string res;

    if (ext)
    {
        res += info;
        res += "(";
        res += ext;
        res += ")|";
        res += ext;
        res += "|";
        int icnt = _GetItemCount(ext, ';');
        if (icnt > 1)
        {
            for (int idx = 0; idx < icnt; ++idx)
            {
                string64 buf;
                _GetItem(ext, idx, buf, ';');

                res += info;
                res += "(";
                res += buf;
                res += ")|";
                res += buf;
                res += "|";
            }
        }
        res += "|";
    }
    else
    {
        res = "All files(*.*)|*.*||";
    }
    xr_strcpy(dest, res.c_str());

    for (u32 i = 0; i < res.size(); ++i)
    {
        if (res[i] == '|')
            dest[i] = '\0';
    }
}

//------------------------------------------------------------------------------
// start_flt_ext = -1-all 0..n-indices
//------------------------------------------------------------------------------

// Vista uses this hook for old-style save dialog
UINT_PTR CALLBACK OFNHookProcOldStyle(HWND, UINT, WPARAM, LPARAM)
{
    // let default hook work on this message
    return 0;
}

bool EFS_Utils::GetOpenNameInternal(HWND hWnd, LPCSTR initial, LPSTR buffer, int sz_buf, bool bMulti, LPCSTR offset, int start_flt_ext)
{
    VERIFY(buffer && (sz_buf > 0));
    FS_Path&   P = *FS.get_path(initial);
    string1024 flt;
    int        flt_cnt = MakeFilter(flt, P.m_FilterCaption ? P.m_FilterCaption : "", P.m_DefExt);

    if (xr_strlen(buffer))
    {
        string_path dr;
        if (!(buffer[0] == '\\' && buffer[1] == '\\'))
        {   // if !network
            _splitpath(buffer, dr, 0, 0, 0);

            if (0 == dr[0])
            {
                string_path bb;
                P._update(bb, buffer);
                xr_strcpy(buffer, sz_buf, bb);
            }
        }
    }

    CFileDialog fd(CFileDialog::fdOpen);
    fd.Caption = "Open a File";

    if (strstr(Core.Params, "-dont_remember_last_folder"))
    {
        string512 path;
        xr_strcpy(path, (offset && offset[0]) ? offset : P.m_Path);
        fd.CurrentDir  = path;
        fd.CurrentFile = buffer;
        fd.DefaultDir  = path;
    }

    fd.SetFilters(flt);
    if (start_flt_ext >= 0)
    {
        if (flt_cnt > 1)
            fd.DefaultFilter = start_flt_ext + 1;   // MakeFilter creates first filter that accepts all extensions
        else
            fd.DefaultFilter = start_flt_ext;
    }
    if (P.m_DefExt)
    {
        string64 defext;
        _GetItem(P.m_DefExt, 0, defext, ';');
        fd.DefaultExt = defext;
    }

    fd.PathMustExist = true;
    fd.FileMustExist = true;
    fd.NoChangeDir   = true;
    fd.Multiselect   = bMulti;
    fd.NoPlacesBar   = true;

    bool bRes        = fd.ShowModal(GetForegroundWindow());
    if (!bRes)
    {
        u32 err = GetLastError();
        switch (err)
        {
            case ERROR_INSUFFICIENT_BUFFER:
                Log("! Too many files selected.");
                break;
        }
        return false;
    }
    if (bMulti)
    {
        buffer[0] = '\0';

        int count = fd.GetResultCount();
        for (int i = 0; i < count; i++)
        {
            strcat_s(buffer, sz_buf, fd.GetResult(i).c_str());
            strcat_s(buffer, sz_buf, ",");
        }
    }
    else if (!fd.GetResult(buffer, sz_buf))
        Log("! File name too long");

    strlwr(buffer);
    return bRes;
}

bool EFS_Utils::GetSaveName(LPCSTR initial, string_path& buffer, LPCSTR offset, int start_flt_ext)
{
    FS_Path&   P = *FS.get_path(initial);
    string1024 flt;

    LPCSTR     def_ext = P.m_DefExt;
    if (false)   //&& dwWindowsMajorVersion == 6 )
    {
        if (strstr(P.m_DefExt, "*."))
            def_ext = strstr(P.m_DefExt, "*.") + 2;
    }

    int flt_cnt = MakeFilter(flt, P.m_FilterCaption ? P.m_FilterCaption : "", def_ext);

    if (xr_strlen(buffer))
    {
        string_path dr;
        if (!(buffer[0] == '\\' && buffer[1] == '\\'))
        {   // if !network
            _splitpath(buffer, dr, 0, 0, 0);
            if (0 == dr[0])
                P._update(buffer, buffer);
        }
    }

    CFileDialog fd(CFileDialog::fdSave);

    fd.Caption = "Save a File";
    fd.SetFilters(flt);
    if (start_flt_ext >= 0)
    {
        if (flt_cnt > 1)
            fd.DefaultFilter = start_flt_ext + 1;
        else
            fd.DefaultFilter = start_flt_ext;
    }
    if (P.m_DefExt)
    {
        string64 defext;
        _GetItem(P.m_DefExt, 0, defext, ';');
        fd.DefaultExt = defext;
    }
    if (strstr(Core.Params, "-dont_remember_last_folder"))
    {
        string512 path;
        xr_strcpy(path, (offset && offset[0]) ? offset : P.m_Path);
        fd.CurrentDir = path;
        fd.DefaultDir = path;
    }

    fd.OverwritePrompt = true;
    fd.NoChangeDir     = true;
    fd.NoPlacesBar     = true;

    bool bRes          = fd.ShowModal(GetForegroundWindow());
    if (!bRes)
    {
        u32 err = GetLastError();
        switch (err)
        {
            case ERROR_INSUFFICIENT_BUFFER:
                Log("! Too many file selected.");
                break;
        }
    }
    if (bRes)
        xr_strcpy(buffer, sizeof(buffer), fd.GetResult().c_str());

    strlwr(buffer);
    return bRes;
}
//----------------------------------------------------
LPCSTR EFS_Utils::AppendFolderToName(LPSTR tex_name, u32 const tex_name_size, int depth, BOOL full_name)
{
    string256 _fn;
    xr_strcpy(tex_name, tex_name_size, AppendFolderToName(tex_name, _fn, sizeof(_fn), depth, full_name));
    return tex_name;
}

LPCSTR
EFS_Utils::AppendFolderToName(LPCSTR src_name, LPSTR dest_name, u32 const dest_name_size, int depth, BOOL full_name)
{
    shared_str tmp      = src_name;
    LPCSTR     s        = src_name;
    LPSTR      d        = dest_name;
    int        sv_depth = depth;
    for (; *s && depth; s++, d++)
    {
        if (*s == '_')
        {
            depth--;
            *d = '\\';
        }
        else
        {
            *d = *s;
        }
    }
    if (full_name)
    {
        *d = 0;
        if (depth < sv_depth)
            xr_strcat(dest_name, dest_name_size, *tmp);
    }
    else
    {
        for (; *s; s++, d++)
            *d = *s;
        *d = 0;
    }
    return dest_name;
}

LPCSTR
EFS_Utils::GenerateName(LPCSTR base_path, LPCSTR base_name, LPCSTR def_ext, LPSTR out_name, u32 const out_name_size)
{
    int         cnt = 0;
    string_path fn;
    if (base_name)
        strconcat(sizeof(fn), fn, base_path, base_name, def_ext);
    else
        xr_sprintf(fn, sizeof(fn), "%s%02d%s", base_path, cnt++, def_ext);

    while (FS.exist(fn))
        if (base_name)
            xr_sprintf(fn, sizeof(fn), "%s%s%02d%s", base_path, base_name, cnt++, def_ext);
        else
            xr_sprintf(fn, sizeof(fn), "%s%02d%s", base_path, cnt++, def_ext);
    xr_strcpy(out_name, out_name_size, fn);
    return out_name;
}

// #endif
