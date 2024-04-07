// LocatorAPI.h: interface for the CLocatorAPI class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ELocatorAPIH
#define ELocatorAPIH
#pragma once

#include "LocatorAPI_defs.h"

class XRCORE_API ELocatorAPI: public ILocatorAPI
{
    friend class FS_Path;

public:

private:
    DEFINE_MAP_PRED(LPCSTR, FS_Path*, PathMap, PathPairIt, pred_str);
    PathMap pathes;

public:
    ELocatorAPI();
    virtual ~ELocatorAPI();
    virtual void     _initialize(u32 flags, LPCSTR target_folder = 0, LPCSTR fs_fname = 0);
    virtual void     _destroy();

    virtual IReader* r_open(LPCSTR initial, LPCSTR N);
    IC IReader*      r_open(LPCSTR N)
    {
        return r_open(0, N);
    }
    virtual void     r_close(IReader*& S);

    virtual IWriter* w_open(LPCSTR initial, LPCSTR N);
    virtual IWriter* w_open_ex(LPCSTR initial, LPCSTR N);
    IC IWriter*      w_open(LPCSTR N)
    {
        return w_open(0, N);
    }
    IC IWriter* w_open_ex(LPCSTR N)
    {
        return w_open_ex(0, N);
    }
    virtual void                   w_close(IWriter*& S);

    virtual const ILocatorAPIFile* exist(LPCSTR N);
    virtual const ILocatorAPIFile* exist(LPCSTR path, LPCSTR name);
    virtual const ILocatorAPIFile* exist(string_path& fn, LPCSTR path, LPCSTR name);
    virtual const ILocatorAPIFile* exist(string_path& fn, LPCSTR path, LPCSTR name, LPCSTR ext);

    virtual BOOL                   can_write_to_folder(LPCSTR path);
    virtual BOOL                   can_write_to_alias(LPCSTR path);
    virtual BOOL                   can_modify_file(LPCSTR fname);
    virtual BOOL                   can_modify_file(LPCSTR path, LPCSTR name);

    virtual BOOL                   dir_delete(LPCSTR initial, LPCSTR N, BOOL remove_files);
    virtual BOOL                   dir_delete(LPCSTR full_path, BOOL remove_files)
    {
        return dir_delete(0, full_path, remove_files);
    }
    virtual void file_delete(LPCSTR path, LPCSTR nm);
    virtual void file_delete(LPCSTR full_path)
    {
        file_delete(0, full_path);
    }
    virtual virtual void file_copy(LPCSTR src, LPCSTR dest);
    virtual void         file_rename(LPCSTR src, LPCSTR dest, bool bOwerwrite = true);
    virtual int          file_length(LPCSTR src);

    virtual time_t       get_file_age(LPCSTR nm);
    virtual void         set_file_age(LPCSTR nm, time_t age);

    virtual BOOL         path_exist(LPCSTR path);
    virtual FS_Path*     get_path(LPCSTR path);
    virtual FS_Path*     append_path(LPCSTR path_alias, LPCSTR root, LPCSTR add, BOOL recursive);
    virtual LPCSTR       update_path(string_path& dest, LPCSTR initial, LPCSTR src);

    virtual BOOL         file_find(LPCSTR full_name, FS_File& f);

    virtual int          file_list(FS_FileSet& dest, LPCSTR path, u32 flags = FS_ListFiles, LPCSTR mask = 0);
    // void              update_path(xr_string &dest, LPCSTR initial, LPCSTR src);
};

#endif   // ELocatorAPIH
