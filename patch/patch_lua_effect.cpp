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

#include "patch_lua_effect.hpp"
#ifdef PATCH_SWITCH_LUA_EFFECT

namespace patch {
    __declspec(naked) void __cdecl lua_effect_t::asm_func() {
        __asm {
            mov     edi, dword ptr[ee.script_efp_ptr]
            mov     edi, dword ptr[edi]                 // *ee.script_efp_ptr
            mov     edi, dword ptr[edi + 0x000000e4]
            mov     dword ptr[ebx + 0x000000e4], edi
            mov     dword ptr[ebx + 0x000000f4], eax
            ret
        }
    }


    BOOL ishexchar(char c) {
        if ('0' <= c) {
            if (c <= '9') return TRUE;
            if (c <= 'f') {
                if ('a' <= c) return TRUE;
                if ('A' <= c && c <= 'F') return TRUE;
            }
        }
        /*
          if ('0' <= c && c <= '9') return TRUE;
          if ('a' <= c && c <= 'f') return TRUE;
          if ('A' <= c && c <= 'F') return TRUE;
        */
        return FALSE;
    }
    int hexchar2number(char c) { // 十六進法の文字であることは保証されている前提
        if (c <= '9') return c - '0';
        if ('a' <= c) return c - 'a' + 10;
        return c - 'A' + 10;
    }

    BOOL __cdecl lua_effect_t::bytestring2byte(uint8_t* dst, int size, char* str) {
        char* ptr = str;
        char* max_ptr = str + size * 2 + 1;
        while (ishexchar(*ptr)) {
            ptr++;
            if (max_ptr < ptr) return FALSE;
        }
        if (ptr < max_ptr - 1) return FALSE;

        for (int i = size; 0 < i; i--) {
            *dst = hexchar2number(str[0]) * 16 + hexchar2number(str[1]);
            dst++; str += 2;
        }
        return TRUE;
    }


} // namespace patch
#endif // ifdef PATCH_SWITCH_LUA_EFFECT
