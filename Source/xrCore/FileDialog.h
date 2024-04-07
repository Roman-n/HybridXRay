#ifndef FILEDIALOG_H
#define FILEDIALOG_H

class CFileDialog
{
public:
    enum
    {
        fdOpen,
        fdSave
    };

    struct SFilter
    {
        xr_string mask, desc;
    };

    xr_string            Caption, DefaultDir, DefaultExt, CurrentDir, CurrentFile;
    int                  Type, DefaultFilter, CurrentFilter;

    xr_vector<SFilter>   Filters;
    xr_vector<xr_string> Results;

    bool                 Multiselect;
    bool                 ShowReadOnly;
    bool                 PathMustExist;
    bool                 FileMustExist;
    bool                 OverwritePrompt;
    bool                 NoChangeDir;
    bool                 NoPlacesBar;

    CFileDialog(int dialog_type);

    bool ShowModal(HWND owner);
    void SetFilters(const char* filters);   // like OPENFILENAME.lpstrFilters

    void AddFilter(const char* mask, const char* desc)
    {
        SFilter flt;
        flt.mask = mask;
        flt.desc = desc;
        Filters.push_back(flt);
    }

    void ClearFilters()
    {
        Filters.clear();
    }

    int GetResultCount()
    {
        return Results.size();
    }

    bool GetResult(char* buffer, size_t buf_sz, size_t idx = 0)
    {
        if (idx < Results.size() && Results[idx].length() < buf_sz)
        {
            strcpy(buffer, Results[idx].c_str());
            return true;
        }
        else
            return false;
    }

    xr_string& GetResult(size_t idx = 0)
    {
        return Results[idx];
    }
private:
    int ShowOld(HWND owner);
    int ShowVista(HWND owner);
};

#endif
