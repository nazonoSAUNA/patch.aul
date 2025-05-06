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

#include "patch_aup_save.hpp"


#ifdef PATCH_SWITCH_AUP_SAVE
namespace patch {

    __declspec(naked) void __cdecl aup_save_t::clamp_edx_0_49() {
        __asm {
            cmp    edx, 49
            jng    skip
            zero :
            xor edx, edx
                skip :
            test   edx, edx
                jl     zero
                ret
        }
    }

} // namespace patch
#endif // ifdef PATCH_SWITCH_AUP_LSAVE