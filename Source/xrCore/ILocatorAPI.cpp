#include "stdafx.h"
#include "ILocatorAPI.h"
// #include "..\BearBundle\BearCore\BearCore.hpp"
ILocatorAPI* xr_FS = NULL;

ILocatorAPI::ILocatorAPI(): dwAllocGranularity(0), dwOpenCounter(0) {}

ILocatorAPI::~ILocatorAPI() {}

BOOL ILocatorAPI::file_find(LPCSTR full_name, FS_File& f)
{
    return 0;
}

int ILocatorAPI::file_list(FS_FileSet& dest, LPCSTR path, u32 flags, LPCSTR mask)
{
    return 0;
}

void           ILocatorAPI::r_close(CStreamReader*& fs) {}

CStreamReader* ILocatorAPI::rs_open(LPCSTR initial, LPCSTR N)
{
    return nullptr;
}

static void Find(xr_vector<xr_string>& list, const char* path, const char* ext, bool full_path, bool find_file = true)
{
    WIN32_FIND_DATA file;
    string_path     full;
    xr_strcpy(full, path);
    xr_strcat(full, "\\");
    xr_strcat(full, ext);
    HANDLE search_handle = FindFirstFile(full, &file);
    if (reinterpret_cast<int>(search_handle) != -1)
    {
        do
        {
            if (file.dwFileAttributes & 32)
            {
                if (!find_file)
                    continue;
            }
            else
            {
                if (find_file)
                    continue;
            }
            if (file.cFileName[0] == TEXT('.'))
            {
                if (file.cFileName[1] == 0 || ((file.cFileName[1] == TEXT('.') && file.cFileName[2] == 0)))
                {
                    continue;
                }
            }
            if (full_path)
            {
                string_path item;
                xr_strcpy(item, path);
                xr_strcat(item, "\\");
                xr_strcat(item, file.cFileName);
                list.push_back(item);
            }
            else
            {
                list.push_back(file.cFileName);
            }
        }
        while (FindNextFile(search_handle, &file));
    }
    FindClose(search_handle);
}

static void FindDirectory(xr_vector<xr_string>& list, const char* path, const char* ext, bool only_root = false)
{
    xr_vector<xr_string> temp;
    Find(temp, path, ext, true, false);
    if (!only_root)
    {
        for (xr_string& i: temp)
        {
            FindDirectory(list, i.c_str(), ext);
        }
    }
    list.insert(list.begin(), temp.begin(), temp.end());
}

static void FindFiles(xr_vector<xr_string>& list, const char* path, const char* ext, bool only_root = false)
{
    xr_vector<xr_string> temp;
    FindDirectory(temp, path, "*", only_root);
    Find(list, path, ext, true);
    for (xr_string& i: temp)
    {
        Find(list, i.c_str(), ext, true);
    }
}

xr_vector<LPSTR>* ILocatorAPI::file_list_open(LPCSTR initial, LPCSTR folder, u32 flags)
{
    xr_vector<LPSTR>* files = xr_new<xr_vector<LPSTR>>();
    string_path       path;
    update_path(path, initial, folder);
    if (flags & FS_ListFiles)
    {
        xr_vector<xr_string> Files;
        if (path[xr_strlen(path) - 1] = '\\')
        {
            path[xr_strlen(path) - 1] = 0;
        }
        FindFiles(Files, path, "*", !(flags & FS_RootOnly));
        for (xr_string& str: Files)
        {
            string_path fname;
            const char* full_path = str.c_str();
            if (strstr(full_path, path))
            {
                full_path += xr_strlen(path) + 1;
            }
            xr_strcpy(fname, full_path);
            if (flags & FS_ClampExt)
                if (0 != strext(fname))
                    *strext(fname) = 0;
            files->push_back(xr_strdup(fname));
        }
    }
    if (flags & FS_ListFolders)
    {
        xr_vector<xr_string> Files;
        if (path[xr_strlen(path) - 1] = '\\')
        {
            path[xr_strlen(path) - 1] = 0;
        }
        FindDirectory(Files, path, "*", !(flags & FS_RootOnly));
        for (xr_string& str: Files)
        {
            string_path fname;
            const char* full_path = str.c_str();
            if (strstr(full_path, path))
            {
                full_path += xr_strlen(path) + 1;
            }
            xr_strcpy(fname, full_path);
            if (flags & FS_ClampExt)
                if (0 != strext(fname))
                    *strext(fname) = 0;
            files->push_back(xr_strdup(fname));
        }
    }
    return files;
}

xr_vector<LPSTR>* ILocatorAPI::file_list_open(LPCSTR path, u32 flags)
{
    return nullptr;
}

void ILocatorAPI::file_list_close(xr_vector<LPSTR>*& lst)
{
    for (LPSTR i: (*lst))
    {
        xr_free(i);
    }
    xr_delete(lst);
}

void ILocatorAPI::auth_generate(xr_vector<shared_str>& ignore, xr_vector<shared_str>& important) {}

u64  ILocatorAPI::auth_get()
{
    return u64();
}

void ILocatorAPI::auth_runtime(void*) {}
