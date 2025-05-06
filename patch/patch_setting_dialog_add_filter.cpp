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

#include "patch_setting_dialog_add_filter.hpp"

#ifdef PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER
namespace patch {

	// Object[idx]のオブジェクトにLoadedFilterTable[filter_id]のフィルタが付与できるかを判定する関数
	bool settingdialog_add_filter_t::is_filter_applicable(int object_idx, int filter_id) {
		if (*reinterpret_cast<uint32_t*>(GLOBAL::exedit_base + OFS::ExEdit::ObjectAllocNum) <= (uint32_t)object_idx) {
			return false;
		}
		if (*reinterpret_cast<uint32_t*>(GLOBAL::exedit_base + OFS::ExEdit::LoadedFilterCount) <= (uint32_t)filter_id) {
			return false;
		}

		auto obj = *reinterpret_cast<ExEdit::Object**>(GLOBAL::exedit_base + OFS::ExEdit::ObjectArrayPointer);
		if (0 <= obj[object_idx].index_midpt_leader) {
			object_idx = obj[object_idx].index_midpt_leader;
		}
		obj = &obj[object_idx];

		auto efpt = reinterpret_cast<ExEdit::Filter**>(GLOBAL::exedit_base + OFS::ExEdit::LoadedFilterTable);

		auto base_flag = efpt[obj->filter_param->id]->flag;
		auto filter_flag = efpt[filter_id]->flag;


		if (has_flag(filter_flag, ExEdit::Filter::Flag::Unaddable)) {
			return false;
		}

		if (reinterpret_cast<bool(__cdecl*)(ExEdit::Object*)>(GLOBAL::exedit_base + OFS::ExEdit::is_camera_object)(obj)) { // カメラ関連
			return reinterpret_cast<bool(__cdecl*)(ExEdit::Filter*)>(GLOBAL::exedit_base + OFS::ExEdit::is_camera_effect)(efpt[filter_id]);
		}
		
		if (has_flag(base_flag, ExEdit::Filter::Flag::Audio) != has_flag(filter_flag, ExEdit::Filter::Flag::Audio)) {
			return false; // 片方のみが音声フィルタなので
		}

		if (has_flag(base_flag, ExEdit::Filter::Flag::Input) || has_flag(base_flag, ExEdit::Filter::Flag::Effect)) { // メディアオブジェクト
			return has_flag(filter_flag, ExEdit::Filter::Flag::Effect) || has_flag(base_flag, ExEdit::Filter::Flag::Input) || has_flag(base_flag, ExEdit::Filter::Flag::Output);
		}

		return !has_flag(filter_flag, ExEdit::Filter::Flag::Effect);
	}

	/*
	   フィルタ効果を追加するためには設定ダイアログの[＋]ボタンや右クリックによるメニュー表示が先に行われるため、
	   その時にexedit+0x1a535cの部分にObject.flagを保存し、複数選択している他オブジェクトにもフィルタ効果を追加できるか判定している
	   しかし、他のプラグインによるフィルタ効果の追加を行う際にはメニュー表示を経由しないためflagが正常に保存されない。
	   それにより付与されるべきオブジェクトに付与されない上、付与できないはずのオブジェクトに付与されることがある。
	*/
	// exedit+0x1a535c に Object.flag を保存する関数
	void settingdialog_add_filter_t::set_last_menu_object_flag(int object_idx) {
		auto obj = *(ExEdit::Object**)(GLOBAL::exedit_base + OFS::ExEdit::ObjectArrayPointer) + object_idx;
		auto object_flag = (ExEdit::Object::Flag*)(GLOBAL::exedit_base + 0x1a535c);
		*object_flag = obj->flag;
	}

	void __stdcall settingdialog_add_filter_t::SendMessageA_wrap(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		int object_idx = *(int*)(GLOBAL::exedit_base + OFS::ExEdit::SettingDialog_ObjIdx);
		if (is_filter_applicable(object_idx, lParam)) {
			set_last_menu_object_flag(object_idx);
			
			SendMessageA(hWnd, WM_COMMAND, wParam, lParam);
		}
	}

	int __cdecl settingdialog_add_filter_t::append_filter_effect_wrap(int object_idx, int filter_idx) {
		if (!is_filter_applicable(object_idx, filter_idx)) {
			return -1;
		}
		return reinterpret_cast<int(__cdecl*)(int, int)>(GLOBAL::exedit_base + OFS::ExEdit::append_filter_effect)(object_idx, filter_idx);
	}

} // namespace patch
#endif // ifdef PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER

