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

#include "init.hpp"

#include <set>
#include <string>

#include "cryptostring.hpp"
#include "util_others.hpp"
#include "util_resource.hpp"

#include "config.hpp"


void init_t::InitAtDllMain() {
	ExchangeFunction(GLOBAL::aviutl_hmod, cstr_kernel32_dll.get(), cstr_EnumResourceLanguagesA.get(), EnumResourceLanguagesA_Wrap);
}

void init_t::InitAtPatchLoaded() {
	{
		static const char aviutl_version_str[] = {
			'1','.','1','0','\0','\0','\0','\0'
		};
		if (memcmp(reinterpret_cast<void*>(GLOBAL::aviutl_base + OFS::AviUtl::VersionString_110), aviutl_version_str, sizeof(aviutl_version_str)) != 0) {
			MessageBoxW(NULL, L"patch.aul requires AviUtl *1.10*.\nAviUtl version 1.10以外では動作しません．", L"patch.aul", MB_ICONEXCLAMATION);
			return;
		}
	}
	GLOBAL::patchaul_path = WinWrap::Module{ GLOBAL::patchaul_hinst }.getFileNameW();
	GLOBAL::patchaul_path_a = WinWrap::Module{ GLOBAL::patchaul_hinst }.getFileNameA();

	GLOBAL::patchaul_config_path = GLOBAL::patchaul_path + L".json";

	if (!PathFileExistsW((LPCWSTR)(GLOBAL::patchaul_config_path.c_str()))) {
		//GLOBAL::config.store(GLOBAL::patchaul_config_path);
		config2.store(GLOBAL::patchaul_config_path);
	}

	//GLOBAL::config.load(GLOBAL::patchaul_config_path);
	config2.load(GLOBAL::patchaul_config_path);

	add_dll_ref.add_ref();

	ModulesData::update();

	{
		DWORD oldProtect;
		VirtualProtect(GLOBAL::executable_memory, sizeof(GLOBAL::executable_memory), PAGE_EXECUTE_READWRITE, &oldProtect);
	}
	InjectFunction_fastcall(GLOBAL::aviutl_base + OFS::AviUtl::InitAuf, InitAufBefore, 5);

	ExchangeFunction(GLOBAL::aviutl_hmod, cstr_kernel32_dll.get(), cstr_LoadLibraryA.get(), LoadLibraryAWrap);

	overwrite_resource();

#ifdef PATCH_SWITCH_CHECK_INIT_COMPLETION
	patch::check_init_completion.init();
#endif

#ifdef PATCH_SWITCH_SPLASH
	if (PATCH_SWITCHER_MEMBER(PATCH_SWITCH_SPLASH)) {
		patch::splash.init();

		patch::splash.set_phase(L"patch.aulの準備中", L"");
		patch::splash.start();
	}
#endif

#ifdef PATCH_SWITCH_CONSOLE
	patch::console.init();
#endif

#ifdef PATCH_SWITCH_EXCEPTION_LOG
	patch::exception_log();
#endif

#ifdef PATCH_SWITCH_SYSINFO_MODIFY
	patch::sysinfo_info_write();
#endif

#ifdef PATCH_SWITCH_KEYCONFIG
	patch::KeyConfig.init();
	patch::ApendKey.init();
#endif

#ifdef PATCH_SWITCH_FAILED_MAX_FRAME
	patch::failed_max_frame.init();
#endif

#ifdef PATCH_SWITCH_ACCESS_KEY
	patch::access_key.init();
#endif

#ifdef PATCH_SWITCH_COLORPALETTE_CACHE
	patch::colorpalette_cache.init();
#endif

#ifdef PATCH_SWITCH_TRACKBAR
	patch::trackbar.init();
#endif

#ifdef PATCH_SWITCH_FILEINFO
	patch::fileinfo.init();
#endif

#ifdef PATCH_SWITCH_SET_FRAME
	patch::set_frame.init();
#endif

#ifdef PATCH_SWITCH_AUDIO_FILTERED_CACHE
	patch::audio_filtered_cache.init();
#endif

#ifdef PATCH_SWITCH_READ_AUDIO
	patch::read_audio.init();
#endif

#ifdef PATCH_SWITCH_WAVE_FILE_READER
	patch::wave_file_reader.init();
#endif

#ifdef PATCH_SWITCH_ADJUST_VMEM
	patch::adjust_vmem.init();
#endif

#ifdef PATCH_SWITCH_UPDATE_COUNT
	patch::update_count.init_au();
#endif
}

void init_t::InitAtExeditLoad() {
	mywindow.init();

#ifdef PATCH_SWITCH_UPDATE_COUNT
	patch::update_count.init_ee();
#endif

#ifdef PATCH_SWITCH_THEME_CC
	patch::theme_cc.init();
#endif
#ifdef PATCH_SWITCH_AVI_FILE_HANDLE_CLOSE
	patch::avi_file_handle_close.init();
#endif
#ifdef PATCH_SWITCH_AVI_FILE_HANDLE_SHARE
	patch::avi_file_handle_share.init();
#endif

#ifdef PATCH_SWITCH_TRA_AVIUTL_FILTER
	patch::tra_aviutlfilter.init();
#endif
#ifdef PATCH_SWITCH_TRA_CHANGE_DRAWFILTER
	patch::tra_change_drawfilter.init();
#endif
#ifdef PATCH_SWITCH_TRA_CHANGE_MODE
	patch::tra_change_mode.init();
#endif
#ifdef PATCH_SWITCH_TRA_ACC_DEC_MOVEMENT
	patch::tra_acc_dec_movement.init();
#endif
#ifdef PATCH_SWITCH_TRA_SPECIFIED_SPEED
	patch::tra_specified_speed.init();
#endif


#ifdef PATCH_SWITCH_AUP_LOAD
	patch::aup_load.init();
#endif
#ifdef PATCH_SWITCH_AUP_LAYER_SETTING
	patch::aup_layer_setting.init();
#endif
#ifdef PATCH_SWITCH_AUP_SCENE_SETTING
	patch::aup_scene_setting.init();
#endif
#ifdef PATCH_SWITCH_LOAD_EXEDIT_BACKUP
	patch::load_exedit_backup.init();
#endif
#ifdef PATCH_SWITCH_EXO_AVIUTL_FILTER
	patch::exo_aviutlfilter.init();
#endif

#ifdef PATCH_SWITCH_EXO_SCENEIDX
	patch::exo_sceneidx.init();
#endif

#ifdef PATCH_SWITCH_EXO_TRACKPARAM
	patch::exo_trackparam.init();
#endif

#ifdef PATCH_SWITCH_EXO_TRACK_MINUSVAL
	patch::exo_trackminusval.init();
#endif

#ifdef PATCH_SWITCH_EXO_MIDPT_AND_TRA
	patch::exo_midpt_and_tra.init();
#endif

#ifdef PATCH_SWITCH_EXO_SPECIALCOLORCONV
	patch::exo_specialcolorconv.init();
#endif

#ifdef PATCH_SWITCH_EXO_ABNORMAL_POS
	patch::exo_abnormal_pos.init();
#endif

#ifdef PATCH_SWITCH_EXO_FOLD_GUI
	patch::exo_fold_gui.init();
#endif

#ifdef PATCH_SWITCH_EXA_CAMERA
	patch::exa_camera.init();
#endif

#ifdef PATCH_SWITCH_EXC_AVI_FILE
	patch::exc_avi_file.init();
#endif

#ifdef PATCH_SWITCH_STR_MINUSVAL
	patch::str_minusval.init();
#endif
#ifdef PATCH_SWITCH_STR2INT
	patch::str2int.init();
#endif

#ifdef PATCH_SWITCH_CONSOLE
	patch::console.init_at_exedit_init();
#endif

#ifdef PATCH_SWITCH_TEXT_OP_SIZE
	patch::text_op_size.init();
#endif
	
#ifdef PATCH_SWITCH_IGNORE_MEDIA_PARAM_RESET
	patch::ignore_media_param_reset.init();
#endif

#ifdef PATCH_SWITCH_SCROLL_OBJDLG
	patch::scroll_objdlg.init();
#endif

#ifdef PATCH_SWITCH_SUSIE_LOAD
	patch::susie_load.init();
#endif

#ifdef PATCH_SWITCH_HELPFUL_MSGBOX
	patch::helpful_msgbox.init();
#endif

#ifdef PATCH_SWITCH_FAILED_SJIS_MSGBOX
	patch::failed_sjis_msgbox.init();
#endif
#ifdef PATCH_SWITCH_FAILED_LONGER_PATH
	patch::failed_longer_path.init();
#endif
#ifdef PATCH_SWITCH_FAILED_FILE_DROP
	patch::failed_file_drop.init();
#endif

#ifdef PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER
	patch::setting_dialog_add_filter.init();
#endif

#ifdef PATCH_SWITCH_OBJ_TEXT
	patch::Text.init();
#endif
#ifdef PATCH_SWITCH_OBJ_CREATE_FIGURE
	patch::CreateFigure.init();
#endif
#ifdef PATCH_SWITCH_OBJ_WAVEFORM
	patch::Waveform.init();
#endif
#ifdef PATCH_SWITCH_OBJ_AUDIOFILE
	patch::AudioFile.init();
#endif
#ifdef PATCH_SWITCH_OBJ_SCENE_AUDIO
	patch::SceneAudio.init();
#endif
#ifdef PATCH_SWITCH_OBJ_COLORCORRECTION
	patch::ColorCorrection.init();
#endif
#ifdef PATCH_SWITCH_OBJ_BLUR
	patch::Blur.init();
#endif
#ifdef PATCH_SWITCH_OBJ_GLOW
	patch::Glow.init();
#endif
#ifdef PATCH_SWITCH_OBJ_SHARP
	patch::Sharp.init();
#endif
#ifdef PATCH_SWITCH_OBJ_MASK
	patch::Mask.init();
#endif
#ifdef PATCH_SWITCH_OBJ_RESIZE
	patch::Resize.init();
#endif
#ifdef PATCH_SWITCH_OBJ_ROTATION
	patch::Rotation.init();
#endif

#ifdef PATCH_SWITCH_OBJ_LENSBLUR
	patch::LensBlur.init();
#endif
#ifdef PATCH_SWITCH_OBJ_IMAGELOOP
	patch::ImageLoop.init();
#endif
#ifdef PATCH_SWITCH_OBJ_NOISE
	patch::Noise.init();
#endif
#ifdef PATCH_SWITCH_OBJ_RADIATIONALBLUR
	patch::RadiationalBlur.init();
#endif
#ifdef PATCH_SWITCH_OBJ_FLASH
	patch::Flash.init();
#endif
#ifdef PATCH_SWITCH_OBJ_DIFFUSELIGHT
	patch::DiffuseLight.init();
#endif
#ifdef PATCH_SWITCH_OBJ_SPECIALCOLORCONV
	patch::SpecialColorConv.init();
#endif
#ifdef PATCH_SWITCH_OBJ_MOTIONBLUR
	patch::MotionBlur.init();
#endif
#ifdef PATCH_SWITCH_OBJ_PORTIONFILTER
	patch::PortionFilter.init();
#endif
#ifdef PATCH_SWITCH_OBJ_GROUPCONTROL
	patch::GroupControl.init();
#endif
#ifdef PATCH_SWITCH_OBJ_VOLUMEFADE
	patch::VolumeFade.init();
#endif
#ifdef PATCH_SWITCH_OBJ_NORMALPLAY
	patch::NormalPlay.init();
#endif

#ifdef PATCH_SWITCH_SETTINGDIALOG_NEXT
	patch::dialog_next.init();
#endif
#ifdef PATCH_SWITCH_SETTINGDIALOG_COLOR_PICKER
	patch::dialog_color_picker.init();
#endif
#ifdef PATCH_SWITCH_SETTINGDIALOG_EXCOLORCONFIG
	patch::excolorconfig.init();
#endif
#ifdef PATCH_SWITCH_SETTINGDIALOG_CHROMAKEY
	patch::dialog_chromakey.init();
#endif
#ifdef PATCH_SWITCH_SETTINGDIALOG_SCRIPTCONTROL
	patch::dialog_scriptcontrol.init();
#endif

#ifdef PATCH_SWITCH_RCLICKMENU_SPLIT
	patch::rclickmenu_split.init();
#endif
#ifdef PATCH_SWITCH_RCLICKMENU_DELETE
	patch::rclickmenu_delete.init();
#endif
#ifdef PATCH_SWITCH_MIDPT_DELETE
	patch::midpt_delete.init();
#endif
#ifdef PATCH_SWITCH_BLEND
	patch::blend.init();
#endif
#ifdef PATCH_SWITCH_ADD_EXTENSION
	patch::add_extension.init();
#endif
#ifdef PATCH_SWITCH_PAGE_SIZE_ALLOC
	patch::page_size_alloc.init();
#endif
#ifdef PATCH_SWITCH_SECOND_CACHE
	patch::second_cache.init();
#endif
#ifdef PATCH_SWITCH_SCENE_CACHE
	patch::scene_cache.init();
#endif
#ifdef PATCH_SWITCH_SCENE_VRAM
	patch::scene_vram.init();
#endif
#ifdef PATCH_SWITCH_AUDIO_EE_MAIN
	patch::audio_ee_main.init();
#endif
#ifdef PATCH_SWITCH_AUDIO_LAYER_END
	patch::audio_layer_end.init();
#endif
#ifdef PATCH_SWITCH_AUDIO_EFPI_INIT
	patch::audio_efpi_init.init();
#endif
#ifdef PATCH_SWITCH_AUDIO_PREPROCESS
	patch::audio_preprocess.init();
#endif
#ifdef PATCH_SWITCH_PLAYBACK_SPEED
	patch::playback_speed.init();
#endif
#ifdef PATCH_SWITCH_PLAYBACK_POS
	patch::playback_pos.init();
#endif
#ifdef PATCH_SWITCH_SETTING_NEW_PROJECT
	patch::setting_new_project.init();
#endif
#ifdef PATCH_SWITCH_SHARED_CACHE
	patch::SharedCache.init();
#endif
#ifdef PATCH_SWITCH_RENDERING
	patch::Rendering.init();
#endif
#ifdef PATCH_SWITCH_YC_RGB_CONV
	patch::yc_rgb_conv.init();
#endif
#ifdef PATCH_SWITCH_CAMERA_SCENE
	patch::camera_scene.init();
#endif
#ifdef PATCH_SWITCH_GROUP_CAMERA_SCENE
	patch::group_camera_scene.init();
#endif
#ifdef PATCH_SWITCH_CHANGE_DISP_SCENE
	patch::change_disp_scene.init();
#endif
#ifdef PATCH_SWITCH_RIGHT_TRACKBAR
	patch::right_trackbar.init();
#endif
#ifdef PATCH_SWITCH_SYNC_PLAY_CURSOR
	patch::sync_play_cursor.init();
#endif
#ifdef PATCH_SWITCH_ANY_OBJ
	patch::any_obj.init();
#endif
#ifdef PATCH_SWITCH_OBJECT_COPY
	patch::object_copy.init();
#endif
#ifdef PATCH_SWITCH_PASTE_POS
	patch::paste_pos.init();
#endif
#ifdef PATCH_SWITCH_SWAP_FILTER
	patch::swap_filter.init();
#endif
#ifdef PATCH_SWITCH_EXCLUSION_FONT
	patch::exclusion_font.init();
#endif
#ifdef PATCH_SWITCH_INIT_WINDOW_POS
	patch::init_window_pos.init();
#endif
#ifdef PATCH_SWITCH_OBJECT_TABLE
	patch::object_table.init();
#endif
#ifdef PATCH_SWITCH_GROUP_ID
	patch::group_id.init();
#endif
#ifdef PATCH_SWITCH_SAT_RGB_SPACE
	patch::sat_rgb_space.init();
#endif
#ifdef PATCH_SWITCH_BORDER_ONLY_TEXT
	patch::borderonly_text.init();
#endif
#ifdef PATCH_SWITCH_GGO_BITMAP_TEXT
	patch::ggo_bitmap_text.init();
#endif
	
	patch::setting_dialog();


#ifdef PATCH_SWITCH_EXFILTER
	patch::exfilter::exfilter.init();
	if (patch::exfilter::exfilter.is_enabled_i()) {
		#ifdef PATCH_SWITCH_EXFILTER_FLASH
			patch::exfilter::Flash.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_CONVEXEDGE
			patch::exfilter::ConvexEdge.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_SHARP
			patch::exfilter::Sharp.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_NOISE
			patch::exfilter::Noise.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_GRADATION
			patch::exfilter::Gradation.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_SPECIALCOLORCONV
			patch::exfilter::SpecialColorConv.init();
		#endif

		#ifdef PATCH_SWITCH_EXFILTER_GLARE
			patch::exfilter::Glare.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_AUDIO_AUF
			patch::exfilter::Audio_auf.init();
		#endif
		#ifdef PATCH_SWITCH_EXFILTER_PLUGINS
			patch::exfilter::Plugins.init();
		#endif
	}
#endif

#ifdef PATCH_SWITCH_FAST
	patch::fast::fast.init();
	if (patch::fast::fast.is_enabled_i()) {
		#ifdef PATCH_SWITCH_FAST_GETPUTPIXELDATA
			patch::fast::getputpixeldata();
		#endif

		#ifdef PATCH_SWITCH_FAST_SETTINGDIALOG
			patch::fast_setting_dialog.init();
		#endif

		#ifdef PATCH_SWITCH_FAST_EXEDITWINDOW
			patch::fast_exeditwindow.init();
		#endif
			
		#ifdef PATCH_SWITCH_FAST_EXFUNC_FILL
			patch::fast::exfunc_fill.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_PIXELFORMAT_CONV
			patch::fast::pixelformat_conv.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_YC_FILTER_EFFECT
			patch::fast::yc_filter_effect.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_DRAWFILTER
			patch::fast::DrawFilter.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_TEXT
			patch::fast::text.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_TEXTBORDER
			patch::fast::textborder.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_CREATE_FIGURE
			patch::fast::create_figure.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_WAVEFORM
			patch::fast::Waveform.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_SHADOW
			patch::fast::Shadow.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_BORDER
			patch::fast::Border.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_BORDERBLUR
			patch::fast::BorderBlur.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_BLUR
			patch::fast::Blur.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_SHARP
			patch::fast::Sharp.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_CONVEXEDGE
			patch::fast::ConvexEdge.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_EXTRACTEDGE
			patch::fast::ExtractEdge.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_GLOW
			patch::fast::Glow.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_DIFFUSELIGHT
			patch::fast::DiffuseLight.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_LIGHTEMISSION
			patch::fast::LightEmission.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_MOTIONBLUR
			patch::fast::MotionBlur.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_COLORDRIFT
			patch::fast::ColorDrift.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_COLORKEY
			patch::fast::Colorkey.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_CHROMAKEY
			patch::fast::Chromakey.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_SPECIALCOLORCONV
			patch::fast::SpecialColorConv.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_WIPE
			patch::fast::Wipe.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_RESIZE
			patch::fast::Resize.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_DIVIDEOBJECT
			patch::fast::DivideObject.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_SCENECHANGE
			patch::fast::SceneChange.init();
		#endif
		#ifdef PATCH_SWITCH_FAST_AUDIO_SPECTRUM
			patch::fast::audio_spectrum.init();
		#endif
		
		#ifdef PATCH_SWITCH_CL
			if (patch::fast::cl.init()) {
				if (patch::fast::cl.is_enabled_i()) {
					#ifdef PATCH_SWITCH_FAST_BLEND
						patch::fast::Blend_cl.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_POLARTRANSFORM
						patch::fast::PolarTransform.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_DISPLACEMENTMAP
						patch::fast::DisplacementMap.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_RADIATIONALBLUR
						patch::fast::RadiationalBlur.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_FLASH
						patch::fast::Flash.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_DIRECTIONALBLUR
						patch::fast::DirectionalBlur.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_LENSBLUR
						patch::fast::LensBlur.init();
					#endif
					#ifdef PATCH_SWITCH_FAST_CONVEXEDGE_CL
						patch::fast::ConvexEdgeCL.init();
					#endif
				}
			}
			else {
				patch_resource_message_w(PATCH_RS_PATCH_CANT_USE_CL, MB_TASKMODAL | MB_ICONEXCLAMATION);
			}
		#endif
	}
#endif

#ifdef PATCH_SWITCH_UNDO
	patch::undo.init();
	if (patch::undo.is_enabled_i()) {
		#ifdef PATCH_SWITCH_UNDO_REDO
			patch::redo.init();
		#endif
		#ifdef PATCH_SWITCH_DIALOG_NEW_FILE
			patch::dialog_new_file.init();
		#endif
	}
#endif

}
		
void init_t::InitAufBefore() {
	patch::aviutl_wndproc_override.go();
}

BOOL WINAPI init_t::EnumResourceLanguagesA_Wrap(HMODULE hModule, LPCSTR lpType, LPCSTR lpName, ENUMRESLANGPROCA lpEnumFunc, LONG_PTR lParam) {
	ExchangeFunction((HMODULE)GLOBAL::aviutl_base, cstr_kernel32_dll.get(), cstr_EnumResourceLanguagesA.get(), EnumResourceLanguagesA);

	InitAtPatchLoaded();

	return FALSE;
}

HMODULE WINAPI init_t::LoadLibraryAWrap(LPCSTR lpLibFileName) {
	HMODULE ret = LoadLibraryA(lpLibFileName);
	if (ret == NULL)return NULL;

	LPCSTR filename = PathFindFileNameA(lpLibFileName);
	if (lstrcmpiA(filename, "exedit.auf") == 0) {
		if (GLOBAL::exedit_hmod != nullptr)return ret;
		if (*reinterpret_cast<int*>(GLOBAL::aviutl_base + OFS::AviUtl::vram_yc_size) == 2)return ret; // YUY2FilterMode
		if (load_i32((uint32_t)ret + OFS::ExEdit::VersionInt32_092) != 9200) {
			MessageBoxW(NULL, L"patch.aul requires Exedit version *0.92*.\n拡張編集 version 0.92以外では動作しません．", L"patch.aul", MB_ICONEXCLAMATION);
			return ret;
		}
		GLOBAL::exedit_hmod = ret;
		auto filters = reinterpret_cast<AviUtl::GetFilterTableList_t>(GetProcAddress(ret, reinterpret_cast<LPCSTR>(GLOBAL::aviutl_base + OFS::AviUtl::str_GetFilterTableList)))();
		original_func_init = std::exchange(filters[0]->func_init, func_initWrap);
		original_func_WndProc = std::exchange(filters[0]->func_WndProc, func_WndProcWrap);
#ifdef _DEBUG
		original_func_proc = std::exchange(filters[0]->func_proc, func_procWrap);
#endif
		InitAtExeditLoad();
	}
#ifdef PATCH_SWITCH_CANCEL_BOOST_CONFLICT
	else if (lstrcmpiA(filename, "Boost.auf") == 0) {
		if (*reinterpret_cast<int*>(GLOBAL::aviutl_base + OFS::AviUtl::vram_yc_size) == 2)return ret; // YUY2FilterMode
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_GetModuleHandleA.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_GetModuleHandleA_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_GetModuleHandleW.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_GetModuleHandleW_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_LoadLibraryA.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_LoadLibraryA_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_LoadLibraryW.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_LoadLibraryW_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_Module32First.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_Module32First_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_Module32FirstW.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_Module32FirstW_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_Module32Next.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_Module32Next_Wrap);
		}
		if (auto ptr = search_import(ret, cstr_kernel32_dll.get(), cstr_Module32NextW.get())) {
			OverWriteOnProtectHelper(ptr, 4).store_i32(0, &init_t::Boost_Module32NextW_Wrap);
		}
	}
#endif
#ifdef PATCH_SWITCH_SCRIPT_SORT_PATCH
	else if (lstrcmpiA(filename, "script_sort.auf") == 0) {
		if (*reinterpret_cast<int*>(GLOBAL::aviutl_base + OFS::AviUtl::vram_yc_size) == 2)return ret; // YUY2FilterMode
		patch::patch_script_sort.init(ret);
	}
#endif
#ifdef PATCH_SWITCH_RELATIVE_PATH_PATCH
	else if (lstrcmpiA(filename, "relative_path.auf") == 0) {
		if (*reinterpret_cast<int*>(GLOBAL::aviutl_base + OFS::AviUtl::vram_yc_size) == 2)return ret; // YUY2FilterMode
		auto filter = reinterpret_cast<AviUtl::GetFilterTable_t>(GetProcAddress(ret, reinterpret_cast<LPCSTR>(GLOBAL::aviutl_base + OFS::AviUtl::str_GetFilterTable)))();
		if (strcmp(filter->information, "相対パスv0.9b by rikky") == 0) {
			patch::patch_relative_path.init(ret);
		}
	}
#endif
#ifdef PATCH_SWITCH_SETTINGDIALOG_CHROMAKEY
	else if (lstrcmpiA(filename, "WideDialog.auf") == 0) {
		if (*reinterpret_cast<int*>(GLOBAL::aviutl_base + OFS::AviUtl::vram_yc_size) == 2)return ret; // YUY2FilterMode
		patch::dialog_chromakey.switching(false);
	}
#endif
#ifdef PATCH_SWITCH_WARNING_OLD_LSW
	/*
	else if (lstrcmpiA(filename, "lwcolor.auc") == 0) {
		static const SHA256 r940_hash(0xc7, 0xe2, 0x51, 0xde, 0xd2, 0xf8, 0x21, 0xcb, 0x1b, 0xc6, 0xb1, 0x9a, 0x66, 0x43, 0xd3, 0x0d, 0xa4, 0xeb, 0xd6, 0x97, 0x1e, 0x34, 0x1a, 0xb2, 0x11, 0xd9, 0x41, 0x1d, 0xcc, 0xbf, 0x9a, 0x18);
		SHA256 hash(lpLibFileName);
		if (hash == r940_hash) {
			auto ret = patch_resource_message_w(PATCH_RS_PATCH_OLD_LSW, MB_ICONEXCLAMATION | MB_YESNO);
			if (ret == IDYES) {
				static cryptostring lsw_url(L"https://scrapbox.io/aviutl/L-SMASH_Works");
				web_confirm(lsw_url.get());
			}
		}
	}*/
	else if (lstrcmpiA(filename, "lwcolor.auc") == 0) {
		auto col = reinterpret_cast<AviUtl::GetColorPluginTable_t>(GetProcAddress(ret, reinterpret_cast<LPCSTR>(GLOBAL::aviutl_base + OFS::AviUtl::str_GetColorPluginTable)))();

		if (col != NULL && col->information != NULL) {
			if (lstrlen("L-SMASH Works Color Space Converter r") <= lstrlen(col->information)) {
				if (strncmp("L-SMASH Works Color Space Converter r", col->information, lstrlen("L-SMASH Works Color Space Converter r")) == 0) {
					char* ptr = (char*)col->information + lstrlen("L-SMASH Works Color Space Converter r");
					int r = 0;
					while ('0' <= *ptr && *ptr <= '9') {
						r = r * 10 + *ptr - '0';
						ptr++;
					}

					if (r == 940) {
						if (lstrlen(lpLibFileName) + 3 < _MAX_PATH) {
							char aui64path[_MAX_PATH];
							lstrcpy(aui64path, lpLibFileName);
							lstrcpy(aui64path + lstrlen(aui64path) - lstrlen("color.auc"), "input64.aui");
							if (PathFileExistsA(aui64path)) {
								r = -940;
							}
						}
					}
					if (0 < r && r <= 940) {
						auto mret = patch_resource_message_w(PATCH_RS_PATCH_OLD_LSW, MB_ICONEXCLAMATION | MB_YESNO);
						if (mret == IDYES) {
							static cryptostring lsw_url(L"https://scrapbox.io/aviutl/L-SMASH_Works");
							web_confirm(lsw_url.get());
						}
					}
				}
			}
		}
	}
#endif
	else {
		static std::set<std::string> list = {
			"bakusoku.auf",
			"eclipse_fast.auf",
			"redo.auf",
			"script_sort_patch.auf",
		};

		std::string check = filename;
		std::transform(check.begin(), check.end(), check.begin(), [](auto c) { return std::tolower(c); });

		if (list.find(check) != list.end()) {
			FreeLibrary(ret);
			
			auto ret = patch_resource_message_w(PATCH_RS_PATCH_CONFLICT_PLUGIN, MB_TASKMODAL | MB_ICONINFORMATION | MB_YESNO, string_convert_A2W(filename));

			if (ret) {
				switch (*ret) {
				case IDYES:
					DeleteFileA(lpLibFileName);
				}
			}

			return NULL;
		}
	}
	return ret;
}
	
BOOL __cdecl init_t::func_WndProcWrap(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp) {
	switch (message) {
		#ifdef PATCH_SWITCH_EXEDITWINDOW_SIZING
		case WM_SIZING:
			if (auto ret = patch::exeditwindow_sizing.wndproc(wparam, lparam) == -1) break;
			else return ret;
		#endif
		case AviUtl::FilterPlugin::WindowMessage::Command:
			#ifdef PATCH_SWITCH_UNDO_REDO
			if (wparam == PATCH_EXEDITMENU_REDO) {
				if (patch::redo.is_enabled_i()) {
					patch::redo.run_redo();
				}
				return TRUE;
			}
			#endif
			break;
		case PATCH_EXEDIT_EXCOMMAND:
			switch (wparam) {
			case PATCH_EXEDIT_EXCOMMAND_ONE_DELAY:
				PostMessageA(hwnd, PATCH_EXEDIT_EXCOMMAND, lparam, 0);
				return FALSE;
			#ifdef PATCH_SWITCH_ANY_OBJ
			case PATCH_EXEDIT_EXCOMMAND_DESELECT_OBJECT_TL_ACTIVATE:
				patch::any_obj_t::deselect_object_tl_activate();
				return FALSE;
			#endif
			}
			break;
	}
	return original_func_WndProc(hwnd, message, wparam, lparam, editp, fp);
}

BOOL __cdecl init_t::func_initWrap(AviUtl::FilterPlugin* fp) {
	if (original_func_init(fp) == FALSE) return FALSE;
	
	#ifdef PATCH_SWITCH_UNDO_REDO
		fp->exfunc->add_menu_item(fp, "やり直す", fp->hwnd, PATCH_EXEDITMENU_REDO, 'Y', AviUtl::ExFunc::AddMenuItemFlag::Ctrl);
	#endif

#ifdef PATCH_SWITCH_LUA
	patch::lua.init();
	
	#ifdef PATCH_SWITCH_LUA_EFFECT
		patch::lua_effect.init();
	#endif

	#ifdef PATCH_SWITCH_LUA_LOAD
		patch::lua_load.init();
	#endif

	#ifdef PATCH_SWITCH_LUA_GETVALUE
		patch::lua_getvalueex.init();
	#endif

	#ifdef PATCH_SWITCH_LUA_RAND
		patch::lua_rand.init();
	#endif

	#ifdef PATCH_SWITCH_LUA_RANDEX
		patch::lua_randex.init();
	#endif

	#ifdef PATCH_SWITCH_LUA_SETANCHOR
		patch::lua_setanchor.init();
	#endif

#endif

	#ifdef PATCH_SWITCH_WARNING_DUPLICATE_PLUGINS
		// 本当はexeditが無くても動くようにするのが良さそうだけど面倒なのでここに
		patch::WarningDuplicate.init();
	#endif

	return TRUE;
}

BOOL __cdecl init_t::func_procWrap(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip) {
	//std::cout << " = = = = = = " << std::endl;
	#ifdef PATCH_SWITCH_ALPHA_BG
		patch::alpha_bg.func_proc(fp, fpip);
	#endif
	return original_func_proc(fp, fpip);
}

#ifdef PATCH_SWITCH_CANCEL_BOOST_CONFLICT
HMODULE WINAPI init_t::Boost_GetModuleHandleA_Wrap(LPCSTR lpModuleName) {
	auto filename = PathFindFileNameA(lpModuleName);
	if (lstrcmpiA(filename, "patch.aul") == 0) {
		return NULL;
	}
	return GetModuleHandleA(lpModuleName);
}

HMODULE WINAPI init_t::Boost_GetModuleHandleW_Wrap(LPCWSTR lpModuleName) {
	auto filename = PathFindFileNameW(lpModuleName);
	if (lstrcmpiW(filename, L"patch.aul") == 0) {
		return NULL;
	}
	return GetModuleHandleW(lpModuleName);
}

HMODULE WINAPI init_t::Boost_LoadLibraryA_Wrap(LPCSTR lpLibFileName) {
	auto filename = PathFindFileNameA(lpLibFileName);
	if (lstrcmpiA(filename, "patch.aul") == 0) {
		return NULL;
	}
	return LoadLibraryA(lpLibFileName);
}

HMODULE WINAPI init_t::Boost_LoadLibraryW_Wrap(LPCWSTR lpLibFileName) {
	auto filename = PathFindFileNameW(lpLibFileName);
	if (lstrcmpiW(filename, L"patch.aul") == 0) {
		return NULL;
	}
	return LoadLibraryW(lpLibFileName);
}

BOOL WINAPI init_t::Boost_Module32First_Wrap(HANDLE hSnapshot, LPMODULEENTRY32 lpme) {
	auto ret = Module32First(hSnapshot, lpme);
	if (ret && lstrcmpiA(lpme->szModule, "patch.aul") == 0) {
		return Module32Next(hSnapshot, lpme);
	}
	return ret;
}

BOOL WINAPI init_t::Boost_Module32FirstW_Wrap(HANDLE hSnapshot, LPMODULEENTRY32W lpme) {
	auto ret = Module32FirstW(hSnapshot, lpme);
	if (ret && lstrcmpiW(lpme->szModule, L"patch.aul") == 0) {
		return Module32NextW(hSnapshot, lpme);
	}
	return ret;
}

BOOL WINAPI init_t::Boost_Module32Next_Wrap(HANDLE hSnapshot, LPMODULEENTRY32 lpme) {
	auto ret = Module32Next(hSnapshot, lpme);
	if (ret && lstrcmpiA(lpme->szModule, "patch.aul") == 0) {
		return Module32Next(hSnapshot, lpme);
	}
	return ret;
}

BOOL WINAPI init_t::Boost_Module32NextW_Wrap(HANDLE hSnapshot, LPMODULEENTRY32W lpme) {
	auto ret = Module32NextW(hSnapshot, lpme);
	if (ret && lstrcmpiW(lpme->szModule, L"patch.aul") == 0) {
		return Module32NextW(hSnapshot, lpme);
	}
	return ret;
}
#endif
