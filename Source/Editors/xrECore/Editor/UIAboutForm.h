#pragma once

class UIAboutForm: public xrUI
{
public:
    UIAboutForm();
    virtual ~UIAboutForm();
    virtual void Draw();

    static void  Update();
    static void  Show();

private:
    static UIAboutForm* Form;
    ref_texture         m_cocr_icon;

    ref_texture         m_discord_icon;
    ref_texture         m_github_icon;
    ref_texture         m_githubHXR_icon;
    ref_texture         m_gitea_icon;
    ref_texture         m_vk_icon;
    ref_texture         m_stalkerue_icon;
};
