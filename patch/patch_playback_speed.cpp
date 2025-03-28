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

#include "patch_playback_speed.hpp"

#ifdef PATCH_SWITCH_PLAYBACK_SPEED
namespace patch {

	/*
	BOOL __cdecl calc_length_if(DWORD ret, ExEdit::Filter* efp) {
		return (LOWORD(efp->processing) - 1 == *reinterpret_cast<int*>(GLOBAL::exedit_base + OFS::ExEdit::SettingDialog_ObjIdx));
	}
	*/

	__declspec(naked) void __cdecl playback_speed_t::asm_func_calc_length_if() {
		__asm {
			mov     edx, dword ptr [ee.x177a10]
			mov     eax, dword ptr [esp + 0x08]
			movzx   eax, word ptr [eax + 0x000000e4]
			dec     eax
			cmp     eax, dword ptr [edx]
			jz      SKIP
			    add     esp, 0x04 ; 前の関数のretになる
			SKIP:
			ret
		}
	}

	__declspec(naked) void __cdecl playback_speed_t::asm_func_calc_length_movie_file() {
		__asm {
			mov     eax, dword ptr [ee.xd7368]
			mov     dword ptr [eax], 0x00000000
			ret
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_calc_length_audio_file() {
		__asm {
			mov     eax, dword ptr [ee.x24de58]
			mov     dword ptr [eax], 0x00000000
			ret
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_calc_length_scene() {
		__asm {
			mov     eax, dword ptr [ee.x230980]
			mov     dword ptr [eax], 0x00000000
			ret
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_calc_length_scene_audio() {
		__asm {
			mov     eax, dword ptr [ee.x2309e0]
			mov     dword ptr [eax], 0x00000000
			ret
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_wndproc_undo_movie_file() {
		__asm {
			cmp     eax, 0x12
			jnz     SKIP
				push    edi
				push    esi
				call    dword ptr [ee.x6900]
				add     esp, 0x08
			SKIP:
			jmp     dword ptr [ee.x618c]
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_wndproc_undo_audio_file() {
		__asm {
			cmp     eax, 0x12
			jnz     SKIP
				push    edi
				push    esi
				call    dword ptr [ee.x902d0]
				add     esp, 0x08
			SKIP:
			jmp     dword ptr [ee.x8fbf8]
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_wndproc_undo_scene() {
		__asm {
			cmp     eax, 0x0F
			jna     jmp_ee_x83770
			cmp     eax, 0x12
			jnz     SKIP
				push    esi
				call    dword ptr [ee.x83cc0]
				add     esp, 0x04
			SKIP:
			jmp     dword ptr [ee.x83797]

			jmp_ee_x83770:
			jmp     dword ptr [ee.x83770]
		}
	}
	__declspec(naked) void __cdecl playback_speed_t::asm_func_wndproc_undo_scene_audio() {
		__asm {
			cmp     edi, 0x0F
			jna     jmp_ee_x84332
			cmp     edi, 0x12
			jnz     SKIP
				push    esi
				call    dword ptr [ee.x848d0]
				add     esp, 0x04
			SKIP:
			jmp     dword ptr [ee.x84359]

			jmp_ee_x84332:
			jmp     dword ptr [ee.x84332]
		}
	}
} // namespace patch
#endif // ifdef PATCH_SWITCH_PLAYBACK_SPEED