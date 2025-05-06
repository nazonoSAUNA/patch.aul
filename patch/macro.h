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

//#define PATCH_INTERNAL

#define PATCH_VERSION_NAME "patch.aul " PATCH_VERSION_STR

#ifdef PATCH_INTERNAL
#define PATCH_VERSION_STR "internal build:" __DATE__ " " __TIME__ ""

#define PATCH_SWITCH_EXCEPTION_LOG
#define PATCH_SWITCH_SYSINFO_MODIFY
#define PATCH_SWITCH_TRANSRATE
#define PATCH_SWITCH_SETTING_GUI


#define PATCH_SWITCH_ACCESS_KEY access_key
#define PATCH_SWITCH_SUSIE_LOAD susie_load
#define PATCH_SWITCH_SPLASH splash
#define PATCH_SWITCH_TRA_AVIUTL_FILTER tra_aviutl_filter
#define PATCH_SWITCH_EXO_AVIUTL_FILTER exo_aviutl_filter
#define PATCH_SWITCH_EXO_TRACK_MINUSVAL exo_track_minusval
#define PATCH_SWITCH_EXO_SCENEIDX exo_sceneidx
#define PATCH_SWITCH_EXO_TRACKPARAM exo_trackparam
#define PATCH_SWITCH_EXO_SPECIALCOLORCONV exo_specialcolorconv
#define PATCH_SWITCH_TEXT_OP_SIZE text_op_size
#define PATCH_SWITCH_IGNORE_MEDIA_PARAM_RESET ignore_media_param_reset
#define PATCH_SWITCH_FONT_DIALOG font_dialog
#define PATCH_SWITCH_SCROLL_OBJDLG scroll_objdlg
#define PATCH_SWITCH_ALPHA_BG alpha_bg
#define PATCH_SWITCH_HELPFUL_MSGBOX helpful_msgbox
#define PATCH_SWITCH_THEME_CC theme_cc
#define PATCH_SWITCH_EXEDITWINDOW_SIZING exeditwindow_sizing
#define PATCH_SWITCH_SETTINGDIALOG_MOVE settingdialog_move
#define PATCH_SWITCH_SETTINGDIALOG_EXCOLORCONFIG settingdialog_excolorconfig

#define PATCH_SWITCH_UNDO undo
#ifdef PATCH_SWITCH_UNDO
	#define PATCH_SWITCH_UNDO_REDO undo_redo
#endif

#define PATCH_SWITCH_CONSOLE console
#ifdef PATCH_SWITCH_CONSOLE

#define PATCH_SWITCH_DEBUGSTRING console_debug_string

#endif // ifdef PATCH_SWITCH_CONSOLE


#define PATCH_SWITCH_LUA lua
#ifdef PATCH_SWITCH_LUA

	#define PATCH_SWITCH_LUA_ENV lua_env
	#define PATCH_SWITCH_LUA_GETVALUE lua_getvalueex
	#define PATCH_SWITCH_LUA_RAND lua_rand
	#define PATCH_SWITCH_LUA_RANDEX lua_randex
	#define PATCH_SWITCH_LUA_PATH lua_path

	//#define PATCH_SWITCH_LUA_COPYBUFFER_SMEM lua_copybuffer_smem


#endif // ifdef PATCH_SWITCH_LUA


#define PATCH_SWITCH_FAST fast
#ifdef PATCH_SWITCH_FAST
	
	#define PATCH_SWITCH_FAST_GETPUTPIXELDATA fast_getputpixeldata
	#define PATCH_SWITCH_FAST_SETTINGDIALOG fast_settingdialog
	#define PATCH_SWITCH_FAST_EXEDITWINDOW fast_exeditwindow

	#define PATCH_SWITCH_CL cl
	#ifdef PATCH_SWITCH_CL
		#define PATCH_SWITCH_FAST_POLORTRANSFORM fast_polortransform
		#define PATCH_SWITCH_FAST_RADIATIONALBLUR fast_radiationalblur
		#define PATCH_SWITCH_FAST_FLASH fast_flash

	#endif // define PATCH_SWITCH_CL

#endif //define PATCH_SWITCH_FAST

#else // ifdef PATCH_INTERNAL
#define PATCH_VERSION_STR "r43_ss_73"

#define PATCH_SWITCH_EXCEPTION_LOG
#define PATCH_SWITCH_SYSINFO_MODIFY

#define PATCH_SWITCH_CHECK_INIT_COMPLETION check_init_completion
#define PATCH_SWITCH_ACCESS_KEY access_key
#define PATCH_SWITCH_KEYCONFIG key_config
#define PATCH_SWITCH_FAILED_MAX_FRAME failed_max_frame
#define PATCH_SWITCH_COLORPALETTE_CACHE colorpalette_cache
#define PATCH_SWITCH_TRACKBAR trackbar
#define PATCH_SWITCH_FILEINFO file_info
#define PATCH_SWITCH_SET_FRAME set_frame
#define PATCH_SWITCH_AUDIO_FILTERED_CACHE audio_filtered_cache
#define PATCH_SWITCH_READ_AUDIO read_audio
#define PATCH_SWITCH_WAVE_FILE_READER wave_file_reader
// #define PATCH_SWITCH_ADJUST_VMEM adjust_vmem
#define PATCH_SWITCH_AVI_FILE_HANDLE_CLOSE avi_file_handle_close
#define PATCH_SWITCH_AVI_FILE_HANDLE_SHARE avi_file_handle_share
#define PATCH_SWITCH_TRA_AVIUTL_FILTER tra_aviutl_filter
#define PATCH_SWITCH_TRA_CHANGE_DRAWFILTER tra_change_drawfilter
#define PATCH_SWITCH_TRA_CHANGE_MODE tra_change_mode
#define PATCH_SWITCH_TRA_ACC_DEC_MOVEMENT tra_acc_dec_movement
#define PATCH_SWITCH_TRA_SPECIFIED_SPEED tra_specified_speed
#define PATCH_SWITCH_AUP_SAVE aup_save
#define PATCH_SWITCH_AUP_LOAD aup_load
#define PATCH_SWITCH_LOAD_EXEDIT_BACKUP load_exedit_backup
#define PATCH_SWITCH_EXO_AVIUTL_FILTER exo_aviutl_filter
// #define PATCH_SWITCH_EXO_TRACK_MINUSVAL exo_track_minusval // str_minusval実装により不要に
#define PATCH_SWITCH_EXO_SCENEIDX exo_sceneidx
#define PATCH_SWITCH_EXO_TRACKPARAM exo_trackparam
#define PATCH_SWITCH_EXO_MIDPT_AND_TRA exo_midpt_tra
#define PATCH_SWITCH_EXO_SPECIALCOLORCONV exo_specialcolorconv
#define PATCH_SWITCH_EXO_ABNORMAL_POS exo_abnormal_pos
#define PATCH_SWITCH_EXO_FOLD_GUI exo_fold_gui
#define PATCH_SWITCH_EXA_CAMERA exa_camera
#define PATCH_SWITCH_EXC_AVI_FILE exc_avi_file
#define PATCH_SWITCH_STR_MINUSVAL str_minusval
#define PATCH_SWITCH_STR2INT str2int
#define PATCH_SWITCH_TEXT_OP_SIZE text_op_size
#define PATCH_SWITCH_IGNORE_MEDIA_PARAM_RESET ignore_media_param_reset
#define PATCH_SWITCH_THEME_CC theme_cc
#define PATCH_SWITCH_EXEDITWINDOW_SIZING exeditwindow_sizing
#define PATCH_SWITCH_SETTINGDIALOG_MOVE settingdialog_move
#define PATCH_SWITCH_SETTINGDIALOG_NEXT settingdialog_next
#define PATCH_SWITCH_SETTINGDIALOG_ADD_FILTER settingdialog_add_filter
#define PATCH_SWITCH_SETTINGDIALOG_COLOR_PICKER settingdialog_color_picker
#define PATCH_SWITCH_SETTINGDIALOG_EXCOLORCONFIG settingdialog_excolorconfig
#define PATCH_SWITCH_SETTINGDIALOG_CHROMAKEY settingdialog_chromakey
#define PATCH_SWITCH_SETTINGDIALOG_SCRIPTCONTROL settingdialog_scriptcontrol
#define PATCH_SWITCH_CANCEL_BOOST_CONFLICT
#define PATCH_SWITCH_WARNING_OLD_LSW
#define PATCH_SWITCH_WARNING_DUPLICATE_PLUGINS
#define PATCH_SWITCH_FAILED_SJIS_MSGBOX failed_sjis
#define PATCH_SWITCH_FAILED_LONGER_PATH failed_longpath
#define PATCH_SWITCH_FAILED_FILE_DROP failed_filedrop
#define PATCH_SWITCH_SUSIE_LOAD susie_load
#define PATCH_SWITCH_SMALL_FILTER small_filter
#define PATCH_SWITCH_OBJ_TEXT obj_text
#define PATCH_SWITCH_OBJ_CREATE_FIGURE obj_create_figure
#define PATCH_SWITCH_OBJ_WAVEFORM obj_waveform
#define PATCH_SWITCH_OBJ_AUDIOFILE obj_audiofile
#define PATCH_SWITCH_OBJ_SCENE_AUDIO obj_sceneaudio
#define PATCH_SWITCH_OBJ_COLORCORRECTION obj_colorcorrection
#define PATCH_SWITCH_OBJ_BLUR obj_blur
#define PATCH_SWITCH_OBJ_GLOW obj_glow
// #define PATCH_SWITCH_OBJ_SHARP obj_sharp fast.sharpにより不要に
#define PATCH_SWITCH_OBJ_MASK obj_mask
#define PATCH_SWITCH_OBJ_RESIZE obj_resize
#define PATCH_SWITCH_OBJ_ROTATION obj_rotation
#define PATCH_SWITCH_OBJ_LENSBLUR obj_lensblur
#define PATCH_SWITCH_OBJ_IMAGELOOP obj_imageloop
#define PATCH_SWITCH_OBJ_NOISE obj_noise
#define PATCH_SWITCH_OBJ_RADIATIONALBLUR obj_radiationalblur
#define PATCH_SWITCH_OBJ_FLASH obj_flash
#define PATCH_SWITCH_OBJ_DIFFUSELIGHT obj_diffuselight
#define PATCH_SWITCH_OBJ_SPECIALCOLORCONV obj_specialcolorconv
#define PATCH_SWITCH_OBJ_MOTIONBLUR obj_motionblur
#define PATCH_SWITCH_OBJ_PORTIONFILTER obj_portionfilter
#define PATCH_SWITCH_OBJ_GROUPCONTROL obj_groupcontrol
#define PATCH_SWITCH_OBJ_NORMALPLAY obj_normalplay
#define PATCH_SWITCH_OBJ_VOLUMEFADE obj_volumefade
#define PATCH_SWITCH_BORDER_ONLY_TEXT border_only_text
#define PATCH_SWITCH_GGO_BITMAP_TEXT ggo_bitmap_text
#define PATCH_SWITCH_RCLICKMENU_SPLIT rclickmenu_split
// #define PATCH_SWITCH_RCLICKMENU_DELETE rclickmenu_delete // undo追加更新により不要に
#define PATCH_SWITCH_MIDPT_DELETE midpt_delete
#define PATCH_SWITCH_BLEND blend
#define PATCH_SWITCH_ADD_EXTENSION add_extension
#define PATCH_SWITCH_DIALOG_NEW_FILE dlg_newfile
#define PATCH_SWITCH_PAGE_SIZE_ALLOC page_size_alloc
#define PATCH_SWITCH_SCENE_CACHE scenecache
#define PATCH_SWITCH_SCENE_VRAM scenevram
// #define PATCH_SWITCH_AUDIO_EE_MAIN audio_ee_main // audio_filtered_cacheに移行
#define PATCH_SWITCH_AUDIO_EFPI_INIT audio_efpi_init
#define PATCH_SWITCH_AUDIO_LAYER_END audio_layer_end
// #define PATCH_SWITCH_AUDIO_PREPROCESS audio_preprocess
#define PATCH_SWITCH_PLAYBACK_SPEED pb_speed
#define PATCH_SWITCH_PLAYBACK_POS pb_pos
#define PATCH_SWITCH_SETTING_NEW_PROJECT setting_newproject
#define PATCH_SWITCH_SHARED_CACHE shared_cache
#define PATCH_SWITCH_RENDERING rendering
#define PATCH_SWITCH_YC_RGB_CONV ycrgbconv
#define PATCH_SWITCH_CAMERA_SCENE camera_scene
#define PATCH_SWITCH_GROUP_CAMERA_SCENE group_camera_scene
#define PATCH_SWITCH_CHANGE_DISP_SCENE change_dispscene
#define PATCH_SWITCH_RIGHT_TRACKBAR right_trackbar
// #define PATCH_SWITCH_SYNC_PLAY_CURSOR sync_play_cursor // 副作用が見つかった&字間行間修正でほぼ起こらないので
#define PATCH_SWITCH_ANY_OBJ any_obj
#define PATCH_SWITCH_OBJECT_COPY object_copy
#define PATCH_SWITCH_PASTE_POS paste_pos
#define PATCH_SWITCH_SWAP_FILTER swap_filter
#define PATCH_SWITCH_EXCLUSION_FONT exclusion_font
#define PATCH_SWITCH_INIT_WINDOW_POS init_window_pos
#define PATCH_SWITCH_OBJECT_TABLE object_table
#define PATCH_SWITCH_GROUP_ID group_id
#define PATCH_SWITCH_SAT_RGB_SPACE sat_rgb
#define PATCH_SWITCH_UPDATE_COUNT update_count
#define PATCH_SWITCH_SCRIPT_SORT_PATCH script_sort_patch
#define PATCH_SWITCH_RELATIVE_PATH_PATCH relative_path_patch


#define PATCH_SWITCH_UNDO undo
#ifdef PATCH_SWITCH_UNDO
	#define PATCH_SWITCH_UNDO_REDO undo_redo
	#ifdef PATCH_SWITCH_UNDO_REDO
		#define PATCH_SWITCH_UNDO_REDO_SHIFT undo_redo_shift
	#endif
#endif

#define PATCH_SWITCH_CONSOLE console
#ifdef PATCH_SWITCH_CONSOLE
	#define PATCH_SWITCH_DEBUGSTRING console_debug_string
#endif // ifdef PATCH_SWITCH_CONSOLE

#define PATCH_SWITCH_LUA lua
#ifdef PATCH_SWITCH_LUA
	#define PATCH_SWITCH_LUA_ENV lua_env
	#define PATCH_SWITCH_LUA_EFFECT lua_effect
	#define PATCH_SWITCH_LUA_LOAD lua_load
	#define PATCH_SWITCH_LUA_RAND lua_rand
	#define PATCH_SWITCH_LUA_RANDEX lua_randex
	#define PATCH_SWITCH_LUA_GETVALUE lua_getvalue
	#define PATCH_SWITCH_LUA_SETANCHOR lua_setanchor
	#define PATCH_SWITCH_LUA_PATH lua_path
#endif // ifdef PATCH_SWITCH_LUA

#define PATCH_SWITCH_EXFILTER exfilter
#ifdef PATCH_SWITCH_EXFILTER
	#define PATCH_SWITCH_EXFILTER_FLASH exfilter_flash
	#define PATCH_SWITCH_EXFILTER_CONVEXEDGE exfilter_convexedge
	#define PATCH_SWITCH_EXFILTER_SHARP exfilter_sharp
	#define PATCH_SWITCH_EXFILTER_NOISE exfilter_noise
	#define PATCH_SWITCH_EXFILTER_GRADATION exfilter_gradation
	#define PATCH_SWITCH_EXFILTER_SPECIALCOLORCONV exfilter_specialcolorconv

	#define PATCH_SWITCH_EXFILTER_GLARE exfilter_glare
	// #define PATCH_SWITCH_EXFILTER_AUDIO_AUF exfilter_audio_auf
	#define PATCH_SWITCH_EXFILTER_PLUGINS exfilter_plugins
#endif //define PATCH_SWITCH_EXFILTER

#define PATCH_SWITCH_FAST fast
#ifdef PATCH_SWITCH_FAST
	
	#define PATCH_SWITCH_FAST_SETTINGDIALOG fast_settingdialog
	#define PATCH_SWITCH_FAST_EXEDITWINDOW fast_exeditwindow
	// #define PATCH_SWITCH_FAST_EXFUNC_FILL exfunc_fill
	// #define PATCH_SWITCH_FAST_PIXELFORMAT_CONV pixelformat_conv // 速くなったけどコスパ悪い（_mm256_pow_ps使用で40kB　他でも使うことがあれば有効にする）
	#define PATCH_SWITCH_FAST_YC_FILTER_EFFECT yc_filter_effect
	#define PATCH_SWITCH_FAST_DRAWFILTER draw_filter
	#define PATCH_SWITCH_FAST_TEXT fast_text
	#define PATCH_SWITCH_FAST_TEXTBORDER fast_text_border
	#define PATCH_SWITCH_FAST_CREATE_FIGURE fast_create_figure
	#define PATCH_SWITCH_FAST_WAVEFORM fast_waveform
	#define PATCH_SWITCH_FAST_SHADOW fast_shadow
	#define PATCH_SWITCH_FAST_BORDER fast_border
	#define PATCH_SWITCH_FAST_BORDERBLUR fast_borderblur
	#define PATCH_SWITCH_FAST_BLUR fast_blur
	#define PATCH_SWITCH_FAST_SHARP fast_sharp
	#define PATCH_SWITCH_FAST_CONVEXEDGE fast_convexedge
	#define PATCH_SWITCH_FAST_EXTRACTEDGE fast_extractedge
	#define PATCH_SWITCH_FAST_GLOW fast_glow
	#define PATCH_SWITCH_FAST_DIFFUSELIGHT fast_diffuselight
	#define PATCH_SWITCH_FAST_LIGHTEMISSION fast_lightemisson
	#define PATCH_SWITCH_FAST_COLORDRIFT fast_colordrift
	#define PATCH_SWITCH_FAST_COLORKEY fast_colorkey
	#define PATCH_SWITCH_FAST_CHROMAKEY fast_chromakey
	#define PATCH_SWITCH_FAST_SPECIALCOLORCONV fast_specialcolorconv
	#define PATCH_SWITCH_FAST_RESIZE fast_resize
	#define PATCH_SWITCH_FAST_DIVIDEOBJECT fast_divideobject
	// #define PATCH_SWITCH_FAST_WIPE fast_wipe
	#define PATCH_SWITCH_FAST_SCENECHANGE fast_scenechange
	// #define PATCH_SWITCH_FAST_AUDIO_SPECTRUM fast_audio_spectrum

	#define PATCH_SWITCH_CL cl
	#ifdef PATCH_SWITCH_CL
		#define PATCH_SWITCH_FAST_POLARTRANSFORM fast_polartransform
		#define PATCH_SWITCH_FAST_DISPLACEMENTMAP fast_displacementmap
		#define PATCH_SWITCH_FAST_RADIATIONALBLUR fast_radiationalblur
		#define PATCH_SWITCH_FAST_FLASH fast_flash
		#define PATCH_SWITCH_FAST_DIRECTIONALBLUR fast_directionalblur
		#define PATCH_SWITCH_FAST_LENSBLUR fast_lensblur
		#define PATCH_SWITCH_FAST_CONVEXEDGE_CL fast_convexedge_cl
	//	#define PATCH_SWITCH_FAST_BLEND fast_blend

	#endif // define PATCH_SWITCH_CL

#endif //define PATCH_SWITCH_FAST

#endif // ifdef PATCH_INTERNAL

#define PATCH_SWITCHER_DEFINE_STRING(s) #s
#define PATCH_SWITCHER_DEFINE_STRINGX(s) PATCH_SWITCHER_DEFINE_STRING(s)
#define PATCH_SWITCHER_DEFINE_VALUE(s) s
#define PATCH_SWITCHER_KEY_JOINNER(a, b) a ## b
#define PATCH_SWITCHER_KEY_JOINNERX(a, b) PATCH_SWITCHER_KEY_JOINNER(a, b)
#define PATCH_SWITCHER_DEFINE(name, def) bool name = def; inline static constexpr const char PATCH_SWITCHER_KEY_JOINNER(key_, name)[] = PATCH_SWITCHER_DEFINE_STRING(name);
#define PATCH_SWITCHER_DEFINE_EX(name, def, key) bool name = def; inline static constexpr const char PATCH_SWITCHER_KEY_JOINNER(key_, name)[] = PATCH_SWITCHER_DEFINE_STRING(key);
#define PATCH_SWITCHER_IF(name) if (load_variable(elm, PATCH_SWITCHER_KEY_JOINNER(key_, name), name)) continue;
#define PATCH_SWITCHER_STORE(name) tj(PATCH_SWITCHER_KEY_JOINNER(key_, name), name);
#define PATCH_SWITCHER_MEMBER(name) GLOBAL::config.switcher.name
