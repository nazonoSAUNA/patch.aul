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

#include "update_count.hpp"

#ifdef PATCH_SWITCH_UPDATE_COUNT
namespace patch {

    int update_count_t::inc_count_return1() {
        count++;
        return 1;
    }
    __declspec(naked) void __cdecl update_count_t::asm_func_filter_switch1() {
        __asm {
            inc     dword ptr [count]
            cmp     dword ptr [edi + 0x40], 0x00
            jz      jmp_au_13f13
            jmp     dword ptr [au.x13f01]

            jmp_au_13f13:
            jmp     dword ptr [au.x13f13]
        }
    }
    __declspec(naked) void __cdecl update_count_t::asm_func_filter_switch2() {
        __asm {
            inc     dword ptr [count]
            cmp     dword ptr [esi + 0x40], 0x00
            jz      jmp_au_2fd19
            jmp     dword ptr [au.x30340]

            jmp_au_2fd19 :
            jmp     dword ptr [au.x2fd19]
        }
    }

    __declspec(naked) void __cdecl update_count_t::asm_func_ee_track_check() {
        __asm {
            mov     ebp, dword ptr [ee.x1e0fa4]
            mov     ebp, dword ptr [ebp]
            inc     dword ptr [count]
            ret
        }
    }

} // namespace patch
#endif // ifdef PATCH_UPDATE_COUNT