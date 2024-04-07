//---------------------------------------------------------------------------

#ifndef EditorPreferencesH
#define EditorPreferencesH
//---------------------------------------------------------------------------
// refs
class UIPropertiesForm;

//---------------------------------------------------------------------------
enum
{
    epoDrawPivot       = (1 << 0),
    epoDrawAnimPath    = (1 << 1),
    epoDrawJoints      = (1 << 2),
    epoDrawBoneAxis    = (1 << 3),
    epoDrawBoneNames   = (1 << 4),
    epoDrawBoneShapes  = (1 << 5),
    epoShowHint        = (1 << 6),
    epoDrawLOD         = (1 << 7),
    epoDiscardInstance = (1 << 8),
    epoDeffLoadRB      = (1 << 9),
    epoDeffLoadCF      = (1 << 10),
    epoSelectInGroup   = (1 << 11),
};
class ECORE_API CCustomPreferences: protected xrUI
{
private:   // User declarations
    UIPropertiesForm* m_ItemProps;

protected:
    enum
    {
        ptBool,
        ptInteger,
        ptFloat,
        ptAngle,
        ptColor,
        ptFlags
    };

    struct SPreference
    {
        typedef SPreference  Self;
        typedef SPreference& SelfRef;

        struct SFlag
        {
            const char* Name;
            u32         Flag;
        };

        const char *Section, *Name;
        int         Type;

        union
        {
            u32*     IntValue;
            float*   FloatValue;
            Flags32* FlagsValue;
        };

        Flags32          FlagsDefault;

        u32              IntDefault, IntMin, IntMax, IntInc;

        float            FloatDefault, FloatMin, FloatMax, FloatInc;
        int              FloatDecimal;

        xr_vector<SFlag> Flags;

        SelfRef          set(const char* section, const char* name, int type, int* value, int def = 0, int mn = 0, int mx = 100, int inc = 1)
        {
            return set(section, name, type, (u32*)value, (u32)def, (u32)mn, (u32)mx, (u32)inc);
        }
        SelfRef set(const char* section, const char* name, int type, u32* value, u32 def = 0, u32 mn = 0, u32 mx = 100, u32 inc = 1)
        {
            Section    = section;
            Name       = name;
            Type       = type;
            IntValue   = value;
            IntDefault = def;
            IntMin     = mn;
            IntMax     = mx;
            IntInc     = inc;

            return *this;
        }
        SelfRef set(const char* section, const char* name, int type, float* value, float def = 0.f, float mn = 0.f, float mx = 1.f, float inc = 0.01f, int dec = 2)
        {
            Section      = section;
            Name         = name;
            Type         = type;
            FloatValue   = value;
            FloatDefault = def;
            FloatMin     = mn;
            FloatMax     = mx;
            FloatInc     = inc;
            FloatDecimal = dec;

            return *this;
        }
        SelfRef set(const char* section, const char* name, int type, Flags32* value, Flags32 def)
        {
            Section      = section;
            Name         = name;
            Type         = type;
            FlagsValue   = value;
            FlagsDefault = def;

            return *this;
        }
        SelfRef addFlag(const char* name, u32 flag)
        {
            SFlag f = {name, flag};
            Flags.push_back(f);

            return *this;
        }
    };

    xr_map<LPCSTR, SPreference> m_Prefs;

public:
    u32        start_w;
    u32        start_h;
    u32        start_maximized;
    // view
    float      view_np;
    float      view_fp;
    float      view_fov;
    // fog
    u32        fog_color;
    float      fog_fogness;
    // camera
    float      cam_fly_speed;
    float      cam_fly_alt;
    float      cam_sens_rot;
    float      cam_sens_move;
    // tools mouse
    float      tools_sens_rot;
    float      tools_sens_move;
    float      tools_sens_scale;
    BOOL       tools_show_move_axis;
    // box pick
    BOOL       bp_lim_depth;
    BOOL       bp_cull;
    float      bp_depth_tolerance;
    // snap
    float      snap_angle;
    float      snap_move;
    float      snap_moveto;
    float      scale_fixed;
    // grid
    float      grid_cell_size;
    u32        grid_cell_count;
    // scene
    u32        scene_undo_level;
    u32        scene_recent_count;
    u32        scene_clear_color;
    AStringVec scene_recent_list;
    // objects
    Flags32    object_flags;
    // environment
    float      env_from_time;
    float      env_to_time;
    float      env_speed;
    shared_str sWeather;

protected:
    void         OnKeyboardCommonFileClick(ButtonValue* value, bool& bModif, bool& bSafe);
    void         OnMotionCompressChanged(PropValue* sender);
    void         OnClose();
    void         ApplyValues();

    virtual void Load(CInifile*);
    virtual void Save(CInifile*);
    virtual void Draw();

public:   // User declarations
    CCustomPreferences();
    virtual ~CCustomPreferences();

    void         OnCreate();
    void         OnDestroy();

    virtual void FillProp(PropItemVec& items);

    void         Edit();

    void         Load();
    void         Save();

    void         AppendRecentFile(LPCSTR name);
    LPCSTR       FirstRecentFile()
    {
        return scene_recent_list.empty() ? "" : scene_recent_list.front().c_str();
    }
};
//---------------------------------------------------------------------------
#define R_FLOAT_SAFE(S, L, D)  I->line_exist(S, L) ? I->r_float(S, L) : D;
#define R_U32_SAFE(S, L, D)    I->line_exist(S, L) ? I->r_u32(S, L) : D;
#define R_BOOL_SAFE(S, L, D)   I->line_exist(S, L) ? I->r_bool(S, L) : D;
#define R_STRING_SAFE(S, L, D) I->line_exist(S, L) ? I->r_string_wb(S, L) : D;
//---------------------------------------------------------------------------
extern ECORE_API CCustomPreferences* EPrefs;
//---------------------------------------------------------------------------

#endif
