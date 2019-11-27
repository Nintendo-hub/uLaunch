#include <ui/ui_ThemeMenuLayout.hpp>
#include <os/os_Account.hpp>
#include <util/util_Convert.hpp>
#include <ui/ui_QMenuApplication.hpp>
#include <fs/fs_Stdio.hpp>

extern ui::QMenuApplication::Ref qapp;
extern cfg::Theme theme;
extern cfg::Config config;

namespace ui
{
    ThemeMenuLayout::ThemeMenuLayout()
    {
        this->SetBackgroundImage(cfg::GetAssetByTheme(theme, "ui/Background.png"));

        pu::ui::Color textclr = pu::ui::Color::FromHex(qapp->GetUIConfigValue<std::string>("text_color", "#e1e1e1ff"));
        pu::ui::Color menufocusclr = pu::ui::Color::FromHex(qapp->GetUIConfigValue<std::string>("menu_focus_color", "#5ebcffff"));
        pu::ui::Color menubgclr = pu::ui::Color::FromHex(qapp->GetUIConfigValue<std::string>("menu_bg_color", "#0094ffff"));

        this->curThemeBanner = pu::ui::elm::Image::New(0, 585, cfg::GetAssetByTheme(theme, "ui/BannerTheme.png"));
        qapp->ApplyConfigForElement("themes_menu", "banner_image", this->curThemeBanner);
        this->Add(this->curThemeBanner);

        this->themesMenu = pu::ui::elm::Menu::New(200, 60, 880, menubgclr, 100, 5);
        this->themesMenu->SetOnFocusColor(menufocusclr);
        qapp->ApplyConfigForElement("themes_menu", "themes_menu_item", this->themesMenu);
        this->Add(this->themesMenu);

        this->curThemeText = pu::ui::elm::TextBlock::New(20, 540, cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_current") + ":", 30);
        this->curThemeText->SetColor(textclr);
        qapp->ApplyConfigForElement("themes_menu", "current_theme_text", this->curThemeText);
        this->Add(this->curThemeText);
        
        this->curThemeName = pu::ui::elm::TextBlock::New(40, 610, "", 30);
        this->curThemeName->SetColor(textclr);
        qapp->ApplyConfigForElement("themes_menu", "current_theme_name_text", this->curThemeName);
        this->Add(this->curThemeName);
        this->curThemeAuthor = pu::ui::elm::TextBlock::New(45, 650, "", 20);
        this->curThemeAuthor->SetColor(textclr);
        qapp->ApplyConfigForElement("themes_menu", "current_theme_author_text", this->curThemeAuthor);
        this->Add(this->curThemeAuthor);
        this->curThemeVersion = pu::ui::elm::TextBlock::New(45, 675, "", 20);
        this->curThemeVersion->SetColor(textclr);
        qapp->ApplyConfigForElement("themes_menu", "current_theme_version_text", this->curThemeVersion);
        this->Add(this->curThemeVersion);
        this->curThemeIcon = pu::ui::elm::Image::New(1000, 605, "");
        this->curThemeIcon->SetWidth(100);
        this->curThemeIcon->SetHeight(100);
        qapp->ApplyConfigForElement("themes_menu", "current_theme_icon", this->curThemeIcon);
        this->Add(this->curThemeIcon);

        this->SetOnInput(std::bind(&ThemeMenuLayout::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    }

    void ThemeMenuLayout::Reload()
    {
        pu::ui::Color textclr = pu::ui::Color::FromHex(qapp->GetUIConfigValue<std::string>("text_color", "#e1e1e1ff"));
        if(cfg::ThemeIsDefault(theme))
        {
            this->curThemeText->SetText(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_no_custom"));
            this->curThemeName->SetVisible(false);
            this->curThemeAuthor->SetVisible(false);
            this->curThemeVersion->SetVisible(false);
            this->curThemeBanner->SetVisible(false);
            this->curThemeIcon->SetVisible(false);
        }
        else
        {
            this->curThemeText->SetText(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_current") + ":");
            this->curThemeName->SetVisible(true);
            this->curThemeName->SetText(theme.manifest.name);
            this->curThemeAuthor->SetVisible(true);
            this->curThemeAuthor->SetText(theme.manifest.author);
            this->curThemeVersion->SetVisible(true);
            this->curThemeVersion->SetText("v" + theme.manifest.release);
            this->curThemeBanner->SetVisible(true);
            this->curThemeIcon->SetVisible(true);
            this->curThemeIcon->SetImage(theme.path + "/theme/Icon.png");
            this->curThemeIcon->SetWidth(100);
            this->curThemeIcon->SetHeight(100);
        }
        this->themesMenu->ClearItems();
        this->themesMenu->SetSelectedIndex(0);

        this->loadedThemes.clear();
        this->loadedThemes = cfg::LoadThemes();

        auto ditm = pu::ui::elm::MenuItem::New(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_reset"));
        ditm->AddOnClick(std::bind(&ThemeMenuLayout::theme_Click, this));
        ditm->SetColor(textclr);
        ditm->SetIcon("romfs:/Logo.png");
        this->themesMenu->AddItem(ditm);
        
        for(auto &ltheme: this->loadedThemes)
        {
            auto itm = pu::ui::elm::MenuItem::New(ltheme.manifest.name + " (v" + ltheme.manifest.release + ", " + cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_by") + " " + ltheme.manifest.author + ")");
            itm->AddOnClick(std::bind(&ThemeMenuLayout::theme_Click, this));
            itm->SetColor(textclr);
            auto iconpath = ltheme.path + "/theme/Icon.png";
            itm->SetIcon(iconpath);
            this->themesMenu->AddItem(itm);
        }
    }

    void ThemeMenuLayout::OnInput(u64 down, u64 up, u64 held, pu::ui::Touch pos)
    {
        bool ret = am::QMenuIsHomePressed();
        if(down & KEY_B) ret = true;
        if(ret)
        {
            qapp->FadeOut();
            qapp->LoadMenu();
            qapp->FadeIn();
        }
    }

    void ThemeMenuLayout::theme_Click()
    {
        auto idx = this->themesMenu->GetSelectedIndex();
        if(idx == 0)
        {
            if(cfg::ThemeIsDefault(theme)) qapp->ShowNotification(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_no_custom"));
            else
            {
                auto sopt = qapp->CreateShowDialog(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_reset"), cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_reset_conf"), { cfg::GetLanguageString(config.main_lang, config.default_lang, "yes"), cfg::GetLanguageString(config.main_lang, config.default_lang, "cancel") }, true);
                if(sopt == 0)
                {
                    config.theme_name = "";
                    cfg::SaveConfig(config);
                    qapp->ShowNotification(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_changed"));
                }
            }
        }
        else
        {
            idx--;
            auto seltheme = this->loadedThemes[idx];
            auto iconpath = seltheme.path + "/theme/Icon.png";
            if(seltheme.base_name == theme.base_name) qapp->ShowNotification(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_active_this"));
            else
            {
                auto sopt = qapp->CreateShowDialog(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_set"), cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_set_conf"), { cfg::GetLanguageString(config.main_lang, config.default_lang, "yes"), cfg::GetLanguageString(config.main_lang, config.default_lang, "cancel") }, true, iconpath);
                if(sopt == 0)
                {
                    config.theme_name = seltheme.base_name;
                    cfg::SaveConfig(config);
                    qapp->ShowNotification(cfg::GetLanguageString(config.main_lang, config.default_lang, "theme_changed"));
                }
            }
        }
    }
}