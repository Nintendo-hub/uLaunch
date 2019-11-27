
#pragma once
#include <q_Include.hpp>

namespace am
{
    namespace controller
    {
        struct InitialArg
        {
            u32 this_size;
            u32 unk1;
            bool unk2; // unk2 (and unk3?) need to be true for controller to launch
            bool unk3;
            u8 mode; // Changing this doesn't seem to affect, but this is the mode according to SDK RE...?
            bool unk4;
            u64 controller_type;
        } PACKED;

        struct MainArg
        {
            u8 min_player_count;
            u8 max_player_count;
            bool take_over_connection;
            bool left_justify;
            bool permit_dual_joy;
            bool single_mode;
            bool use_colors;
            u32 colors[4];
            bool use_controller_names;
            char controller_names[4][0x80];
        } PACKED;
    }

    bool LibraryAppletIsActive();
    bool LibraryAppletIsQMenu();
    void LibraryAppletTerminate();
    Result LibraryAppletStart(AppletId id, u32 la_version, void *in_data, size_t in_size);
    Result LibraryAppletSend(void *data, size_t size);
    Result LibraryAppletRead(void *data, size_t size);
    Result WebAppletStart(WebCommonConfig *web);
    Result LibraryAppletQMenuLaunchWith(AppletId id, u32 la_version, std::function<void(AppletHolder*)> on_prepare, std::function<void(AppletHolder*)> on_finish, std::function<bool()> on_wait);

    inline Result LibraryAppletQMenuLaunchWithSimple(AppletId id, u32 la_version, void *in_data, size_t in_size, void *out_data, size_t out_size, std::function<bool()> on_wait)
    {
        return LibraryAppletQMenuLaunchWith(id, la_version,
        [&](AppletHolder *h)
        {
            if(in_size > 0) libappletPushInData(h, in_data, in_size);
        },
        [&](AppletHolder *h)
        {
            if(out_size > 0) libappletPopOutData(h, out_data, out_size, NULL);
        }, on_wait);
    }

    AppletId LibraryAppletGetId();

    static constexpr AppletId QMenuAppletId = AppletId_shop;
    static constexpr AppletId QHbTargetAppletId = AppletId_auth;
}