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

#ifdef PATCH_SWITCH_OBJ_GROUPCONTROL
#include <memory>

#include <exedit.hpp>

#include "global.hpp"
#include "offset_address.hpp"
#include "util.hpp"
#include "config_rw.hpp"

namespace patch {

    // init at exedit load
    // グループ制御がチェックで無効になっている際にフィルタ効果が実行されるのを修正
    inline class obj_GroupControl_t {
        bool enabled = true;
        bool enabled_i;
        inline static const char key[] = "obj_groupcontrol";


        inline static struct _ofs {
            int32_t xa6ba8 = 0xa6ba8;
            int32_t x497a4 = 0x497a4;
            int32_t x4997e = 0x4997e;
        }ee;
        static void __cdecl asm_func();
        // add_base(GLOBAL::exedit_base, &ee, sizeof(ee));

    public:
        void init() {
            enabled_i = enabled;

            if (!enabled_i)return;

            add_base(GLOBAL::exedit_base, &ee, sizeof(ee));

            ReplaceNearJmp(GLOBAL::exedit_base + 0x049782, &asm_func);
            /*
                10049780 0f84f8010000       jz      1004997e
                ↓
                10049780 0f84XxXxXxXx       jz      cursor

                10000000 81fdXxXxXxXx       cmp     ebp,ee+a6ba8 ;efGroupControl
                10000000 0f84XxXxXxXx       jz      ee+497a4
                10000000 e9XxXxXxXx         jmp     ee+4997e
            */

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
    } GroupControl;
} // namespace patch

#endif // ifdef PATCH_SWITCH_OBJ_GROUPCONTROL
