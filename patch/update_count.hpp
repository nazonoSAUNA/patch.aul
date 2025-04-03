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

#ifdef PATCH_SWITCH_UPDATE_COUNT

#include <aviutl.hpp>
#include <exedit.hpp>

#include "global.hpp"
#include "util.hpp"
#include "restorable_patch.hpp"

#include "config_rw.hpp"

namespace patch {
    // init at aviutl or exedit load
    // トラックバーを動かすときなど、動かし初めしかset_undoされなかったりするため、変更が分からないことがある
    // ここではトラックの変化やチェックの変化などが検出できる（全ての変更は検出できないので他はundo_id_max等を使う）

    inline class update_count_t {

        static int inc_count_return1();

        bool enabled = true;
        bool enabled_i;

        inline static const char key[] = "update_count";

        inline static int count = 0;


        inline static struct _ofs_au {
            int32_t x13f01 = 0x13f01;
            int32_t x13f13 = 0x13f13;
            int32_t x2fd19 = 0x2fd19;
            int32_t x30340 = 0x30340;
        }au;
        static void __cdecl asm_func_filter_switch1();
        static void __cdecl asm_func_filter_switch2();
        // add_base(GLOBAL::aviutl_base, &au, sizeof(au));
        inline static struct _ofs_ee {
            int32_t x1e0fa4 = 0x1e0fa4;
        }ee;
        static void __cdecl asm_func_ee_track_check();
        // add_base(GLOBAL::exedit_base, &ee, sizeof(ee));


    public:

        void init_au() {
            enabled_i = enabled;

            if (!enabled_i)return;

            add_base(GLOBAL::aviutl_base, &au, sizeof(au));
            {
                /* 各プラグインのトラックバーやチェックなど
                    00430da9 b801000000         mov     eax,00000001
                    ↓
                    00430da9 e8XxXxXxXx         call    nesfunc_return_1
                */
                OverWriteOnProtectHelper h(GLOBAL::aviutl_base + 0x30da9, 5);
                h.store_i8(0, '\xe8');
                h.replaceNearJmp(1, &inc_count_return1);
            }
            { // フィルタの有効/無効
                {
                    /*
                        00413efb 837f4000           cmp     dword ptr [edi+40],+00
                        00413eff 7412               jz      413f13
                        00413f01
                        ↓
                        00413efb e9XxXxXxXx         jmp     cursor

                        cursor00 ff05XxXxXxXx       inc     dword ptr [count]
                        cursor06 837f4000           cmp     dword ptr [edi+40],+00
                        cursor0a 0f84XxXxXxXx       jz      au+13f13
                        cursor10 e9XxXxXxXx         jmp     au+13f01
                    */
                    OverWriteOnProtectHelper h(GLOBAL::aviutl_base + 0x13efb, 5);
                    h.store_i8(0, '\xe9');
                    h.replaceNearJmp(1, &asm_func_filter_switch1);
                }
                {
                    /*
                        00430336 837e4000           cmp     dword ptr [esi+40],+00
                        0043033a 0f84d9f9ffff       jz      0042fd19
                        ↓
                        00430336 e9XxXxXxXx         jmp     cursor
                        00430340

                        cursor00 ff05XxXxXxXx       inc     dword ptr [count]
                        cursor06 837e4000           cmp     dword ptr [esi+40],+00
                        cursor0a 0f84XxXxXxXx       jz      au+2fd19
                        cursor10 e9XxXxXxXx         jmp     au+30340

                    */
                    OverWriteOnProtectHelper h(GLOBAL::aviutl_base + 0x30336, 5);
                    h.store_i8(0, '\xe9');
                    h.replaceNearJmp(1, &asm_func_filter_switch2);
                }
            }
        }
        void init_ee() {

            if (!enabled_i)return;

            add_base(GLOBAL::exedit_base, &ee, sizeof(ee));
            /* 拡張編集のトラックバーやチェックなど
                100357ea 8b2da40f1e10       mov     ebp,dword ptr [ExEdit.ObjectArrayPointer]
                ↓
                100357ea 90                 nop
                100357eb e8XxXxXxXx         call    cursor

                cursor00 8b2dXxXxXxxx       mov     ebp,dword ptr [ExEdit.ObjectArrayPointer]
                cursor06 ff05XxXxXxXx       inc     dword ptr [count]
                cursor0c c3                 ret

            */
            OverWriteOnProtectHelper h(GLOBAL::exedit_base + 0x357ea, 6);
            h.store_i16(0, '\x90\xe8');
            h.replaceNearJmp(2, &asm_func_ee_track_check);
        }

        int get() {
            return count;
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

    } update_count;
} // namespace patch
#endif // ifdef PATCH_SWITCH_UPDATE_COUNT
