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

#include "patch_obj_groupcontrol.hpp"

#ifdef PATCH_SWITCH_OBJ_GROUPCONTROL
namespace patch {

	__declspec(naked) void __cdecl obj_GroupControl_t::asm_func() {
		__asm {
			cmp     ebp, dword ptr [ee.xa6ba8]
			jnz     jump_ee_x4997e
				jmp     dword ptr [ee.x497a4]
			jump_ee_x4997e:
			jmp     dword ptr [ee.x4997e]
		}
	}
} // namespace patch
#endif // ifdef PATCH_SWITCH_OBJ_GROUPCONTROL
