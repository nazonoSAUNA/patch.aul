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

#ifdef PATCH_SWITCH_LUA_EFFECT

#include "global.hpp"
#include "offset_address.hpp"
#include "util.hpp"
#include "config_rw.hpp"


namespace patch {

    // init at exedit load
    // obj.effect("filter_name")にてefp->proccesingが変わらないのを修正
    // exdataで使われる文字列変換の過程で、問題のある文字列操作をしているのを修正 例；obj.effect("縁取り","color","ffffff")

    inline class lua_effect_t {
        bool enabled = true;
        bool enabled_i;
        inline static const char key[] = "lua.effect";

        static BOOL __cdecl bytestring2byte(uint8_t* dst, int size, char* str);

        inline static struct _ofs {
            int32_t script_efp_ptr = OFS::ExEdit::script_efp;
        }ee;
        static void __cdecl asm_func();

    public:
        void init() {
            enabled_i = enabled;

            if (!enabled_i)return;

            add_base(GLOBAL::exedit_base, &ee, sizeof(ee));

            { // obj.effect("filter_name")にてefp->proccesingが変わらないのを修正
                /*
                    1005d20e 8983f4000000       mov     dword ptr [ebx+000000f4],eax
                    ↓
                    1005d20e 90                 nop
                    1005d20f e8XxXxXxXx         call    cursor

                    10000000 8b3dXxXxXxXx       mov     edi,dword ptr [ee+1b2b10]
                    10000000 8bbfe4000000       mov     edi,dword ptr [edi+000000e4]
                    10000000 89bbe4000000       mov     dword ptr [ebx+000000e4],edi
                    10000000 8983f4000000       mov     dword ptr [ebx+000000f4],eax
                    10000000 c3                 ret
                */

                OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x5d20e, 6);
                h.store_i16(0, '\x90\xe8');
                h.replaceNearJmp(2, &asm_func);

            }
            { // exdataで使われる文字列変換の過程で、問題のある文字列操作をしているのを修正
                ReplaceNearJmp(GLOBAL::exedit_base + 0x5d4c3, &bytestring2byte);
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
    } lua_effect;
} // namespace patch

#endif // ifdef PATCH_SWITCH_LUA_EFFECT
