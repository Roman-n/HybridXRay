#pragma once
#include "LocatorAPI_defs.h"
struct ILocatorAPIFile
{
    LPCSTR name;              // low-case name
    u32    vfs;               // 0xffffffff - standart file
    u32    crc;               // contents CRC
    u32    ptr;               // pointer inside vfs
    u32    size_real;         //
    u32    size_compressed;   // if (size_real==size_compressed) - uncompressed
    u32    modif;             // for editor
};
class XRCORE_API CStreamReader;
class XRCORE_API ILocatorAPI
{
    friend class FS_Path;

public:
    ILocatorAPI();
    virtual ~ILocatorAPI();
    virtual void     _initialize(u32 flags, LPCSTR target_folder = 0, LPCSTR fs_fname = 0) = 0;
    virtual void     _destroy()                                                            = 0;

    virtual IReader* r_open(LPCSTR initial, LPCSTR N)                                      = 0;
    virtual void     r_close(IReader*& S)                                                  = 0;
    IC IReader*      r_open(LPCSTR N)
    {
        return r_open(0, N);
    }

    virtual IWriter* w_open(LPCSTR initial, LPCSTR N)    = 0;
    virtual IWriter* w_open_ex(LPCSTR initial, LPCSTR N) = 0;
    virtual void     w_close(IWriter*& S)                = 0;
    IC IWriter*      w_open(LPCSTR N)
    {
        return w_open(0, N);
    }
    IC IWriter* w_open_ex(LPCSTR N)
    {
        return w_open_ex(0, N);
    }

    virtual const ILocatorAPIFile* exist(LPCSTR N)                                              = 0;
    virtual const ILocatorAPIFile* exist(LPCSTR path, LPCSTR name)                              = 0;
    virtual const ILocatorAPIFile* exist(string_path& fn, LPCSTR path, LPCSTR name)             = 0;
    virtual const ILocatorAPIFile* exist(string_path& fn, LPCSTR path, LPCSTR name, LPCSTR ext) = 0;

    virtual BOOL                   can_write_to_folder(LPCSTR path)                             = 0;
    virtual BOOL                   can_write_to_alias(LPCSTR path)                              = 0;
    virtual BOOL                   can_modify_file(LPCSTR fname)                                = 0;
    virtual BOOL                   can_modify_file(LPCSTR path, LPCSTR name)                    = 0;

    virtual BOOL                   dir_delete(LPCSTR initial, LPCSTR N, BOOL remove_files)      = 0;
    virtual BOOL                   dir_delete(LPCSTR full_path, BOOL remove_files)
    {
        return dir_delete(0, full_path, remove_files);
    }
    virtual void file_delete(LPCSTR path, LPCSTR nm) = 0;
    virtual void file_delete(LPCSTR full_path)
    {
        file_delete(0, full_path);
    }
    virtual void              file_copy(LPCSTR src, LPCSTR dest)                                      = 0;
    virtual void              file_rename(LPCSTR src, LPCSTR dest, bool bOwerwrite = true)            = 0;
    virtual int               file_length(LPCSTR src)                                                 = 0;

    virtual time_t            get_file_age(LPCSTR nm)                                                 = 0;
    virtual void              set_file_age(LPCSTR nm, time_t age)                                     = 0;

    virtual BOOL              path_exist(LPCSTR path)                                                 = 0;
    virtual FS_Path*          get_path(LPCSTR path)                                                   = 0;
    virtual FS_Path*          append_path(LPCSTR path_alias, LPCSTR root, LPCSTR add, BOOL recursive) = 0;
    virtual LPCSTR            update_path(string_path& dest, LPCSTR initial, LPCSTR src)              = 0;

    virtual BOOL              file_find(LPCSTR full_name, FS_File& f);
    virtual int               file_list(FS_FileSet& dest, LPCSTR path, u32 flags = FS_ListFiles, LPCSTR mask = 0);
    virtual void              r_close(CStreamReader*& fs);
    virtual CStreamReader*    rs_open(LPCSTR initial, LPCSTR N);

    virtual xr_vector<LPSTR>* file_list_open(LPCSTR initial, LPCSTR folder, u32 flags = FS_ListFiles);
    virtual xr_vector<LPSTR>* file_list_open(LPCSTR path, u32 flags = FS_ListFiles);
    virtual void              file_list_close(xr_vector<LPSTR>*& lst);

    virtual void              auth_generate(xr_vector<shared_str>& ignore, xr_vector<shared_str>& important);
    virtual u64               auth_get();
    virtual void              auth_runtime(void*);

    u32                       dwAllocGranularity;
    Flags32                   m_Flags;
    u32                       dwOpenCounter;

    enum
    {
        flNeedRescan       = (1 << 0),
        flBuildCopy        = (1 << 1),
        flReady            = (1 << 2),
        flEBuildCopy       = (1 << 3),
        flEventNotificator = (1 << 4),
        flTargetFolderOnly = (1 << 5),
        flCacheFiles       = (1 << 6),
        flScanAppRoot      = (1 << 7),
        flNeedCheck        = (1 << 8),
        flDumpFileActivity = (1 << 9),
    };
    //.    void						update_path			(xr_string& dest, LPCSTR initial, LPCSTR src);
};

extern XRCORE_API ILocatorAPI* xr_FS;
#define FS (*xr_FS)
