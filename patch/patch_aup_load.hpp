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

#ifdef PATCH_SWITCH_AUP_LOAD

#include <exedit.hpp>

#include "global.hpp"
#include "util.hpp"
#include "restorable_patch.hpp"

#include "config_rw.hpp"

namespace patch {
    // init at exedit load
    // プロジェクト読み込み時のバグ修正

    // (未) 異常なオブジェクトを削除するように変更

    // 編集プロジェクト読み込み時にシーン設定の一部情報（グリッド設定など）が欠けるのを修正

    inline class aup_load_t {
        //static int __cdecl func_project_load_end();

        bool enabled = true;
        bool enabled_i;

        inline static const char key[] = "aup_load";

        inline static struct _ofs {
            int32_t x1e0fa0 = 0x1e0fa0;
        }ee;
        static void __cdecl asm_func_select_idx();
        // add_base(GLOBAL::exedit_base, &ee, sizeof(ee));

    public:

        void init() {
            enabled_i = enabled;

            if (!enabled_i)return;

            add_base(GLOBAL::exedit_base, &ee, sizeof(ee));

            { // 選択中のオブジェクトのIDが異常値になっている時にエラーとなるのを修正
                /*
                    100325a1 8b4718             mov     eax,dword ptr [edi+18]
                    100325a4 83c414             add     esp,+14
                    100325a7 85c0               test    eax,eax
                    ↓
                    100325a1 83c414             add     esp,+14
                    100325a4 e8XxXxXxXx         call    cursor

                    10000000 8b4718             mov     eax,dword ptr [edi+18]
                    10000000 3b05XxXxXxXx       cmp     eax,dword ptr [ee+1e0fa0]
                    10000000 7c06               jl      skip,06
                    10000000 83c8ff             or      eax,0xffffffff
                    10000000 894718             mov     dword ptr [edi+18],eax
                    10000000 85c0               test    eax,eax
                    10000000 c3                 ret

                */

                OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x325a1, 8);
                h.store_i32(0, '\x83\xc4\x14\xe8');
                h.replaceNearJmp(4, &asm_func_select_idx);
            }
            /*
            { // 読み込み後の最後に実行する関数を追加
                OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x3261c, 5);
                h.store_i8(0, '\xe8');
                h.replaceNearJmp(1, &func_project_load_end);
            }
            */

            { // 編集プロジェクト読み込み時にシーン設定の一部情報（グリッド設定など）が欠けるのを修正
                /*
                    10031b48 8b4648             mov     eax,dword ptr [esi+48]
                    10031b4b c1e205             shl     edx,05
                    10031b4e 8982587a1710       mov     dword ptr [edx+10177a58],eax
                    10031b54 ...
                    ↓
                    10031b48 8bc7               mov     eax,edi
                    10031b4a 90                 nop
                    10031b4b c1e205             shl     edx,05
                    10031b4e 8dbaXxXxXxXx       lea     edi,dword ptr [edx+exedit+177a58] ; Xの部分は書きかえなければ良い
                    10031b54 ...       load_scene_setting_all
                */
                char load_scene_setting_all[] = {
                    "\x83\xc6\x48"             // add     esi,+48
                    "\xb9\x15\x00\x00\x00"     // mov     ecx,00000015 ; こっちは21(22項目目はこの後に別の判定がある)
                    "\xf3\xa5"                 // rep     movsd
                    "\x8b\xf8"                 // mov     edi,eax
                    "\x81\xee\x9c\x00\x00\x00" // sub     esi,0000009c ; 21*4 + 0x48
                    "\x81\xfb\x2b\x23\x00\x00" // cmp     ebx,0000232b
                    "\xeb\x74"                 // jmp     skip,74 (10031be2)
                };
                OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x031b49, 37);
                h.store_i16(0, '\xc7\x90');
                h.store_i16(5, '\x8d\xba');
                memcpy(reinterpret_cast<void*>(h.address(11)), load_scene_setting_all, sizeof(load_scene_setting_all) - 1);
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

    } aup_load;
} // namespace patch
#endif // ifdef PATCH_SWITCH_AUP_LOAD
