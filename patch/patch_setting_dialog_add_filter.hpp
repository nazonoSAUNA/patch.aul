/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include "macro.h"

#ifdef PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER

#include <exedit.hpp>

#include "global.hpp"
#include "offset_address.hpp"
#include "util.hpp"

#include "config_rw.hpp"

namespace patch {

    // init at exedit load
    // 主にプラグインで本来付けられないフィルタ効果を付与できてしまうのを修正

    inline class settingdialog_add_filter_t {

        static bool is_filter_applicable(int object_idx, int filter_idx);
        static void set_last_menu_object_flag(int object_idx);
        static void __stdcall SendMessageA_wrap(HWND hWnd, WPARAM wParam, LPARAM lParam);
        static int __cdecl append_filter_effect_wrap(int object_idx, int filter_idx);

        bool enabled = true;
        bool enabled_i;
        inline static const char key[] = "settingdialog_add_filter";
    public:
        void init() {
            enabled_i = enabled;
            if (!enabled_i)return;

            {
                /*
                    1002d927 6811010000         push    00000111 ;WM_COMMAND
                    1002d92c 50                 push    eax ;exedit_hwnd
                    1002d92d ffd5               call    ebp ;SendMessageA
                    ↓
                    1002d927 50                 push    eax
                    1002d928 6690               nop
                    1002d92a e8XxXxXxXx         call    newfunc
                */
                OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x2d927, 8);
                h.store_i32(0, '\x50\x66\x90\xe8');
                h.replaceNearJmp(4, &SendMessageA_wrap);
            }
            { // exa
                ReplaceNearJmp(GLOBAL::exedit_base + 0x2a22e, &append_filter_effect_wrap);
            }

        }

        void switching(bool flag) {
            enabled = flag;
        }

        bool is_enabled() { return enabled; }
        bool is_enabled_i() { return enabled_i; }

        void switch_load(ConfigReader& cr) {
            cr.regist(key, [this](json_value_s* value) {
                ConfigReader::load_variable(value, enabled);
                });
        }

        void switch_store(ConfigWriter& cw) {
            cw.append(key, enabled);
        }
    } setting_dialog_add_filter;
} // namespace patch

#endif // ifdef PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER
