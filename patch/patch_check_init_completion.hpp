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

#ifdef PATCH_SWITCH_CHECK_INIT_COMPLETION

#include <aviutl.hpp>

#include "global.hpp"
#include "offset_address.hpp"
#include "util.hpp"

#include "config_rw.hpp"

namespace patch {

    // init at patch load
    // AviUtlの初期化処理中に終了処理を行わないように変更

    inline class check_init_completion_t {

        bool enabled = true;
        bool enabled_i;
        inline static const char key[] = "check_init_completion";

#define STATUS_BEFORE_INITIALIZATION 0
#define STATUS_INITIALIZING 1
#define STATUS_AFTER_INITIALIZATION 2
        inline static int status = STATUS_BEFORE_INITIALIZATION;

        // initの最後に実行される
        inline static void __fastcall __report_securityfailure_wrap(int param) {
            status = STATUS_AFTER_INITIALIZATION;
            reinterpret_cast<void(__fastcall*)(int)>(GLOBAL::aviutl_base + OFS::AviUtl::__report_securityfailure)(param);
        }

        inline static void(__cdecl*exit_org)();
        inline static void __cdecl exit_wrap() {
            if (status != STATUS_INITIALIZING) {
                exit_org();
            }
        }

    public:
        void init() {
            enabled_i = enabled;
            if (!enabled_i)return;

            status = STATUS_INITIALIZING;

            ReplaceNearJmp(GLOBAL::aviutl_base + 0x0041ab, &__report_securityfailure_wrap);
            (exit_org) = reinterpret_cast<decltype(exit_org)>(load_rel32(GLOBAL::aviutl_base + 0x007912));
            ReplaceNearJmp(GLOBAL::aviutl_base + 0x007912, &exit_wrap);
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
    } check_init_completion;
} // namespace patch

#endif // ifdef PATCH_SWITCH_CHECK_INIT_COMPLETION
