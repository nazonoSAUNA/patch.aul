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

#include "patch_obj_audiofile.hpp"
#ifdef PATCH_SWITCH_OBJ_AUDIOFILE

namespace patch {

#define TRACK_POS 0
#define TRACK_SPEED 1
#define CHECK_LOOP 0
#define CHECK_SYNC 1

    struct TrackInfo {
        int value;
        int value_s;
        int value_e;
        int scale;
        int value_def;
        HWND trackbar_hwnd;
        HWND edit_hwnd;
        HWND updown_hwnd;
        int pos_y;
        int scale2;
    };


    struct ObjectInfo {
        int frame_start;
        int frame_end;
        int* track_left;
        int* track_right;
        int* track_mode;
        int* check;
        void* exdata;
        int* track_param;
    };

    struct Exdata_MovieFile {
        char path[_MAX_PATH];
        int id_and_count;
        int	TickCount;
        int frame_n;
        int	video_rate;
        int	video_scale;
        int	current_frame;
    };

    struct Exdata_AudioFile {
        char path[_MAX_PATH];
        int id_and_count;
        int	TickCount;
        int centi_sec;
        int	current_frame;
        int	current_pos;
    };

    BOOL __cdecl AudioFile_t::set_trackvalue_wrap8f9b5(ExEdit::Filter* efp, int track_s, int track_e, int scale) {
        ExEdit::ObjectFilterIndex ofi = efp->processing;
        int obj_idx = ((int)ofi & 0xffff) - 1;

        int* SettingDialog_ObjIdx = (int*)(GLOBAL::exedit_base + OFS::ExEdit::SettingDialog_ObjIdx);
        if (*SettingDialog_ObjIdx != obj_idx) {
            return 0;
        }

        ExEdit::Object** ObjectArrayPointer = (ExEdit::Object**)(GLOBAL::exedit_base + OFS::ExEdit::ObjectArrayPointer);
        int track_id = (*ObjectArrayPointer)[obj_idx].filter_param[(int)ofi >> 16].track_begin + 1;

        if (scale == 0) {
            scale = 1;
        }

        TrackInfo* trackinfol = (TrackInfo*)(GLOBAL::exedit_base + OFS::ExEdit::TrackLeftInfoArray);
        TrackInfo* trackinfor = (TrackInfo*)(GLOBAL::exedit_base + OFS::ExEdit::TrackRightInfoArray);

        trackinfor[track_id].value_s = trackinfol[track_id].value_s = efp->track_s[1];
        trackinfor[track_id].value_e = trackinfol[track_id].value_e = efp->track_e[1];
        trackinfor[track_id].scale2 = trackinfol[track_id].scale2 = scale;

        SendMessageA(trackinfol[track_id].trackbar_hwnd, 0x407,
            0, efp->track_drag_min[1] / trackinfol[track_id].scale2);
        SendMessageA(trackinfol[track_id].trackbar_hwnd, 0x408,
            1, efp->track_drag_max[1] / trackinfol[track_id].scale2);
        SendMessageA(trackinfor[track_id].trackbar_hwnd, 0x407,
            0, efp->track_drag_min[1] / trackinfor[track_id].scale2);
        SendMessageA(trackinfor[track_id].trackbar_hwnd, 0x408,
            1, efp->track_drag_max[1] / trackinfor[track_id].scale2);
        SendMessageA(trackinfol[track_id].updown_hwnd, 0x46f,
            efp->track_s[1] / trackinfol[track_id].scale2, efp->track_e[1] / trackinfol[track_id].scale2);
        SendMessageA(trackinfor[track_id].updown_hwnd, 0x46f,
            efp->track_s[1] / trackinfor[track_id].scale2, efp->track_e[1] / trackinfor[track_id].scale2);

        reinterpret_cast<void(__cdecl*)(int)>(GLOBAL::exedit_base + 0x2c1e0)(track_id);

        return 1;
    }
    
    char* __cdecl AudioFile_t::update_dialog_wrap(ExEdit::Filter* efp, void* exdata) {
        ObjectInfo objinfo;

        if (efp->check[CHECK_SYNC] == 0) { // 連携
            auto ofi = reinterpret_cast<ExEdit::ObjectFilterIndex(__cdecl*)(ExEdit::ObjectFilterIndex, char*)>(GLOBAL::exedit_base + OFS::ExEdit::get_above_object)(efp->processing, reinterpret_cast<char*>(GLOBAL::exedit_base + OFS::ExEdit::str_DOUGAFILE));
            if ((int)ofi != 0) {
                efp->exfunc->getvalue(ofi, &objinfo);
                if (((Exdata_MovieFile*)objinfo.exdata)->path[0] != '\0') {
                    lstrcpyA(((Exdata_AudioFile*)exdata)->path, ((Exdata_MovieFile*)objinfo.exdata)->path);

                    double v_sync_rate = 100.0 * (double)((Exdata_MovieFile*)objinfo.exdata)->video_scale / (double)((Exdata_MovieFile*)objinfo.exdata)->video_rate;

                    ((Exdata_AudioFile*)exdata)->centi_sec = (int)((double)((Exdata_MovieFile*)objinfo.exdata)->frame_n * v_sync_rate);

                    if (efp->exfunc->count_section_num(efp->processing) == 1) {
                        efp->track_mode[TRACK_POS] = objinfo.track_mode[TRACK_POS];

                        efp->track_value_left[TRACK_SPEED] = objinfo.track_left[TRACK_SPEED];
                        efp->track_value_left[TRACK_POS] = (int)((double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate);
                        efp->track_value_right[TRACK_POS] = (int)((double)(objinfo.track_right[TRACK_POS] - 1) * v_sync_rate);
                        efp->exfunc->x00(efp->processing); // update_object_data
                    }
                }
            }
        }
        return reinterpret_cast<char*(__cdecl*)(ExEdit::Filter*, void*)>(GLOBAL::exedit_base + 0x8f960)(efp, exdata);
    }

    double __cdecl AudioFile_t::calc_pos_wrap1(ExEdit::ObjectFilterIndex ofi, int milliframe, int video_rate, int video_scale, ExEdit::Filter* efp) {
        double pos = calc_pos(ofi, milliframe, video_rate, video_scale, efp);
        if (efp->track_value_left[TRACK_SPEED] < 0) pos = -pos;
        return pos;
    }
    double __cdecl AudioFile_t::calc_pos_wrap2(ExEdit::ObjectFilterIndex ofi, int milliframe, int video_rate, int video_scale, ExEdit::Filter* efp) {
        /* 
            長さを調整する条件を満たしていたら、この後にある分岐条件である
            if (exdata->centi_sec < objinfo.track_left[TRACK_POS] + (int)(pos * 100.0))
            となるようなposを返す
        */
        ObjectInfo objinfo;
        int centi_sec = ((Exdata_AudioFile*)efp->exdata_ptr)->centi_sec;
        efp->exfunc->getvalue(efp->exfunc->get_start_idx(efp->processing), &objinfo);
        double pos = 100.0 * calc_pos(ofi, milliframe, video_rate, video_scale, efp);
        if (0 <= objinfo.track_left[TRACK_SPEED]) {
            pos += objinfo.track_left[TRACK_POS];
        } else {
            pos += centi_sec - objinfo.track_left[TRACK_POS];
        }
        double ret = (centi_sec - objinfo.track_left[TRACK_POS]) * 0.01 - 1.0;
        if (0 <= efp->track_value_left[TRACK_SPEED]) {
            if (centi_sec < pos) {
                ret += 2.0;
            }
        } else {
            if (pos <= 0) {
                ret += 2.0;
            }
        }
        return ret;
    }
    double __cdecl AudioFile_t::calc_pos_wrap3(ExEdit::ObjectFilterIndex ofi, int milliframe, int video_rate, int video_scale, ExEdit::Filter* efp) {
        ObjectInfo objinfo;
        int centi_sec = ((Exdata_AudioFile*)efp->exdata_ptr)->centi_sec;
        efp->exfunc->getvalue(efp->exfunc->get_start_idx(efp->processing), &objinfo);
        double pos = calc_pos(ofi, milliframe, video_rate, video_scale, efp);

        if (efp->track_value_left[TRACK_SPEED] < 0) {
            pos = -pos;
        }
        if ((efp->track_value_left[TRACK_SPEED] ^ objinfo.track_left[TRACK_SPEED]) < 0) {
            pos += ((double)centi_sec - (double)objinfo.track_left[TRACK_POS] * 2.0) * 0.01;
        }
        return pos;
    }

    void audio_fill_zero(ExEdit::FilterProcInfo* efpip) {
        memset(efpip->audio_data, 0, efpip->audio_n * efpip->audio_ch * sizeof(short));
    }

    BOOL __cdecl AudioFile_t::func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip0) {

        struct FilterProcInfo_1_2 {
            enum Flag : uint32_t {
                bit3 = 1 << 3,
                bit6 = 1 << 6, // efMovieFileなど
                fast_preview = 1 << 9,
                early_filter = 1 << 10, // obj.effect()(引数なし) などを実行するときに設定される
                effect_noarg = 1 << 11, // obj_effect_noarg するときに設定される
                bit12 = 1 << 12, // video_func_main

            } flag;
            void* frame_edit;
            void* frame_temp;
            int32_t scene_w;
            int32_t scene_h;
            int32_t scene_line; // scene_maxw
            int32_t scene_maxh;
            int32_t frame;
            int32_t frame_n;
            int32_t project_w;
            int32_t project_h;
            int16_t* audio_p;
            int32_t audio_n;
            int32_t audio_ch;
            void* pixelp;
            void* editp;
            int32_t yc_size;
            int32_t line_size;
            int32_t unknown[24];
            int32_t frame_num; // タイムライン上の現在フレーム
            ExEdit::PixelYCA* obj_edit;
            ExEdit::PixelYCA* obj_temp;
            int32_t obj_w;
            int32_t obj_h;
            struct Geometry {
                int32_t ox, oy, oz;
                int32_t rx, ry, rz;
                int32_t cx, cy, cz;
                int32_t zoom;
                int32_t aspect;
                int32_t alpha;
            } obj_data;
            int32_t obj_line; // obj_max_w
            int32_t obj_max_h;
            int32_t xf4; // 直前オブジェクトが有効かどうか？
            enum class ObjectFlag {
                bit16 = 1 << 16
            } object_flag;
            int32_t framerate_nu;
            int32_t framerate_de;
            int16_t* audio_data;
            int16_t* audio_temp;
            int32_t audio_rate;
            int32_t add_frame; // frame_numに対して計算上の加算
            int32_t subframe; // 計算上の加算subframe
            int32_t object_start_frame;
            int32_t v_func_idx;
            int32_t obj_index;
            int32_t obj_num;
            int32_t* layer_clipping_flag; // 1:上のオブジェクト, 2:クリッピングするオブジェクト
            int32_t* layer_subframe; // 時間制御を考慮したsubframe
            ExEdit::Object** layer_effect_objectp;
            ExEdit::Object** layer_group_objectp;
            ExEdit::Object* obj_layerp;
            ExEdit::Object* objectp;
            int32_t unknown5;
            struct PolyData {
                int32_t x, y, z, u, v;
            }polydata[4];
            int32_t tick_count;
            ExEdit::ObjectFilterIndex scene_ofi;
            int32_t audio_milliframe;
            int32_t audio_speed; // 等速=1000000
            int32_t sub_size_x; // (root_w - fpip->w) / 2
            int32_t sub_size_y; // (root_h - fpip->h) / 2
            int32_t clipping_data_exists;
        } *efpip = (FilterProcInfo_1_2*)efpip0;


        AviUtl::FileInfo* fip;
        ObjectInfo objinfo;

        double audio_rate = (double)efpip->audio_rate;
        int milliframe, currentframe;
        if (efpip->audio_speed) {
            milliframe = efpip->audio_milliframe + (efpip->frame - efpip->frame_num) * 1000;
            audio_rate *= 1000000.0 / (double)efpip->audio_speed;
            currentframe = (int)round((double)milliframe * 1000 / (double)efpip->audio_speed);
        } else {
            milliframe = efpip->frame * 1000;
            currentframe = efpip->frame;
        }

        auto exdata = (Exdata_AudioFile*)efp->exdata_ptr;
        double framerate = (double)efpip->framerate_nu / (double)efpip->framerate_de;
        double pos_cs;

        double playback_s, playback_e;
        double speed = 1.0;
        double v_sync_rate;
        int loop = efp->check[CHECK_LOOP];
        if (efp->check[CHECK_SYNC]) { // 連携
            auto ofi = reinterpret_cast<ExEdit::ObjectFilterIndex(__cdecl*)(ExEdit::ObjectFilterIndex, char*)>(GLOBAL::exedit_base + OFS::ExEdit::get_above_object)(efp->processing, reinterpret_cast<char*>(GLOBAL::exedit_base + OFS::ExEdit::str_DOUGAFILE));
            if ((int)ofi == 0) {
                if (exdata->path[0] != '\0') {
                    reinterpret_cast<void(__cdecl*)(void*)>(GLOBAL::exedit_base + OFS::ExEdit::avi_handle_close)(exdata);
                    reinterpret_cast<char* (__cdecl*)(ExEdit::Filter*, void*)>(GLOBAL::exedit_base + OFS::ExEdit::efAudioFile_update_dialog)(efp, exdata);
                }
                return FALSE;
            }

            efp->exfunc->getvalue(ofi, &objinfo);
            loop = objinfo.check[CHECK_LOOP];
            if (((Exdata_MovieFile*)objinfo.exdata)->path[0] == '\0') return FALSE;
            if (lstrcmpA(((Exdata_MovieFile*)objinfo.exdata)->path, exdata->path) != 0) {
                if (reinterpret_cast<AviUtl::AviFileHandle * (__cdecl*)(void*, char*, int, AviUtl::FileInfo**)>(GLOBAL::exedit_base + OFS::ExEdit::avi_handle_reopen)(exdata, ((Exdata_MovieFile*)objinfo.exdata)->path, 0x10000020, &fip) == NULL) {
                    return FALSE;
                }
                exdata->centi_sec = 0;
                reinterpret_cast<char* (__cdecl*)(ExEdit::Filter*, void*)>(GLOBAL::exedit_base + OFS::ExEdit::efAudioFile_update_dialog)(efp, exdata);
            }

            if (objinfo.track_mode[TRACK_POS] != 0 && objinfo.track_mode[TRACK_POS] != 1) return FALSE;

            ExEdit::ObjectFilterIndex ofi2 = ofi = efp->exfunc->get_start_idx(ofi);
            while ((int)ofi2 != 0) {
                efp->exfunc->getvalue(ofi2, &objinfo);
                if (efpip->frame_num < objinfo.frame_start) break;
                ofi = ofi2;
                ofi2 = efp->exfunc->x08(ofi); // get_next_obj
            }

            efp->exfunc->getvalue(ofi, &objinfo);
            v_sync_rate = 100.0 * (double)((Exdata_MovieFile*)objinfo.exdata)->video_scale / (double)((Exdata_MovieFile*)objinfo.exdata)->video_rate;
            exdata->centi_sec = (int)((double)((Exdata_MovieFile*)objinfo.exdata)->frame_n * v_sync_rate);

            if (objinfo.track_mode[TRACK_POS] == 0) {
                playback_s = 0.0;
                playback_e = (double)((Exdata_MovieFile*)objinfo.exdata)->frame_n * v_sync_rate;
                speed = (double)objinfo.track_left[TRACK_SPEED] * 0.001;

                efp->exfunc->getvalue(efp->exfunc->get_start_idx(ofi), &objinfo);
                if (0 <= objinfo.track_left[TRACK_SPEED]) {
                    pos_cs = (double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate;
                } else {
                    pos_cs = playback_e - (double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate;
                }
                pos_cs *= 0.01;
                pos_cs += calc_pos(ofi, milliframe, efpip->framerate_nu, efpip->framerate_de, efp);

            } else {
                if (objinfo.track_left[TRACK_POS] == objinfo.track_right[TRACK_POS]) return FALSE;
                if (loop) {
                    speed = (double)objinfo.track_left[TRACK_SPEED] * 0.001;
                    if (objinfo.track_left[TRACK_POS] < objinfo.track_right[TRACK_POS]) {
                        playback_s = (double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate;
                        playback_e = (double)(objinfo.track_right[TRACK_POS] - 1) * v_sync_rate;
                    } else {
                        playback_s = (double)(objinfo.track_right[TRACK_POS] - 1) * v_sync_rate;
                        playback_e = (double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate;
                        speed = -speed;
                    }
                } else {
                    playback_s = (double)(objinfo.track_left[TRACK_POS] - 1) * v_sync_rate;
                    playback_e = (double)(objinfo.track_right[TRACK_POS] - 1) * v_sync_rate;
                    if (playback_s > playback_e) {
                        std::swap(playback_s, playback_e);
                    }
                    speed = (double)(objinfo.track_right[TRACK_POS] - objinfo.track_left[TRACK_POS]) * 0.01 * v_sync_rate * framerate / (double)(objinfo.frame_end - objinfo.frame_start);
                }
                pos_cs = (double)(objinfo.track_left[TRACK_POS] - 1) * 0.01 * v_sync_rate + (double)(milliframe + (efp->frame_start_chain - objinfo.frame_start) * 1000) * speed / (framerate * 1000.0);
                
            }
        } else if (efp->track_mode[TRACK_POS] == 0) { // 移動なし
            playback_s = 0.0;
            playback_e = (double)exdata->centi_sec;
            speed = (double)efp->track_value_left[TRACK_SPEED] * 0.001;

            efp->exfunc->getvalue(efp->exfunc->get_start_idx(efp->processing), &objinfo);
            if (0 <= objinfo.track_left[TRACK_SPEED]) {
                pos_cs = (double)objinfo.track_left[TRACK_POS];
            } else {
                pos_cs = playback_e - (double)objinfo.track_left[TRACK_POS];
            }
            pos_cs *= 0.01;
            pos_cs += calc_pos(efp->processing, milliframe, efpip->framerate_nu, efpip->framerate_de, efp);
        } else { // playback_pos easing
            playback_s = (double)efp->track_value_left[TRACK_POS];
            playback_e = (double)efp->track_value_right[TRACK_POS];
            if (loop) {
                speed = (double)efp->track_value_left[TRACK_SPEED] * 0.001;
                if (playback_s > playback_e) {
                    std::swap(playback_s, playback_e);
                    speed = -speed;
                }
            } else {
                if (playback_s > playback_e) {
                    std::swap(playback_s, playback_e);
                }
                speed = (double)(efp->track_value_right[TRACK_POS] - efp->track_value_left[TRACK_POS]) * framerate / ((double)((efp->frame_end - efp->frame_start) * 100));
            }
            pos_cs = (double)efp->track_value_left[TRACK_POS] * 0.01 + (double)(milliframe + (efp->frame_start_chain - efp->frame_start) * 1000) * speed / (framerate * 1000.0);
        }
        int speed_sign = 1;
        if (speed < 0.0) {
            speed_sign = -1;
            speed = -speed;
        }
        if (speed < 0.015625 || 64.0 < speed) {
            audio_fill_zero((ExEdit::FilterProcInfo*)efpip);
            return TRUE;
        }

        AviUtl::AviFileHandle* afh = reinterpret_cast<AviUtl::AviFileHandle * (__cdecl*)(Exdata_AudioFile*, int, AviUtl::FileInfo**)>(GLOBAL::exedit_base + OFS::ExEdit::avi_handle_open)(exdata, 0x20, &fip);
        if (afh == NULL) return FALSE;

        audio_rate /= (double)speed;
        int audio_rate_i = (int)round(audio_rate);

        if (fip->audio_rate != audio_rate_i || fip->audio_ch != efpip->audio_ch) {
            fip->audio_rate = audio_rate_i;
            fip->audio_ch = efpip->audio_ch;
            fip->audio_n = efp->aviutl_exfunc->avi_file_set_audio_sample_rate(afh, audio_rate_i, efpip->audio_ch);
            exdata->current_frame = -1;
        }


        /*
        if (exdata->current_frame == current_frame && current_frame != 0 && 0 < speed_sign) {
            pos = exdata->current_pos;
        } else {
            double length;
            if ((int)pos_ofi == NULL) {
                length = (double)efp->track_value_left[TRACK_POS] * v_sync_rate * 0.01 + (double)(milliframe + (efp->frame_start_chain - efp->frame_start) * 1000) * (double)(speed * speed_sign) / (framerate * 1000.0);
            } else {
                efp->exfunc->getvalue(efp->exfunc->get_start_idx(pos_ofi), &objinfo);
                if (0 <= objinfo.track_left[TRACK_SPEED]) {
                    length = (double)objinfo.track_left[TRACK_POS] * v_sync_rate;
                } else {
                    length = (double)playback_e - (double)objinfo.track_left[TRACK_POS] * v_sync_rate;
                }
                length *= 0.01;
                length += reinterpret_cast<double(__cdecl*)(ExEdit::ObjectFilterIndex, int, int, int, ExEdit::Filter*)>(GLOBAL::exedit_base + OFS::ExEdit::efAudioFile_calc_pos)(pos_ofi, milliframe, efpip->framerate_nu, efpip->framerate_de, efp);
            }
            pos = (int)std::round(length * (double)audio_rate);
        }

        if (0 < speed_sign) {
            exdata->current_frame = current_frame + 1;
            exdata->current_pos = pos + efpip->audio_n;
        }
        */

        playback_s *= audio_rate * 0.01;
        playback_e *= audio_rate * 0.01;

        double pos;
        if (exdata->current_frame == currentframe && currentframe != 0) {
            pos = exdata->current_pos;
        } else {
            pos = pos_cs * audio_rate;
        }
        exdata->current_frame = currentframe + 1;
        exdata->current_pos = pos + speed_sign * efpip->audio_n;
        if (loop) {
            double length = playback_e - playback_s;
            if (length <= 0.0) {
                exdata->current_frame = -1;
                audio_fill_zero((ExEdit::FilterProcInfo*)efpip);
                return TRUE;
            }
            if (efp->check[CHECK_SYNC]) {
                length += v_sync_rate * audio_rate * 0.01;
            }
            while (pos < playback_s) {
                pos += length;
            }
            while (playback_e <= pos) {
                pos -= length;
            }
        } else if ((double)pos < playback_s || playback_e < (double)pos) {
            exdata->current_frame = -1;
            audio_fill_zero((ExEdit::FilterProcInfo*)efpip);
            return TRUE;
        }
        int read_size = efp->aviutl_exfunc->avi_file_read_audio_sample(afh, (int)pos, speed_sign * efpip->audio_n, efpip->audio_data);

        if (read_size <= 0) {
            audio_fill_zero((ExEdit::FilterProcInfo*)efpip);
            return TRUE;
        }

        if (loop) {
            while (read_size < efpip->audio_n) {
                int n = efp->aviutl_exfunc->avi_file_read_audio_sample(afh, (int)playback_s, speed_sign * (efpip->audio_n - read_size), (short*)efpip->audio_data + efpip->audio_ch * read_size);
                if (n <= 0) {
                    break;
                }
                read_size += n;
            }
        }
        if (read_size < efpip->audio_n) {
            memset((short*)efpip->audio_data + read_size * efpip->audio_ch, 0, (efpip->audio_n - read_size) * efpip->audio_ch * sizeof(short));
        }
        return TRUE;
    }

    __declspec(naked) void __cdecl AudioFile_t::asm_func_if_speed_1() {
        __asm {
            cmp     dword ptr [ecx + 0x04], -0x64
            jg      jump_ee_x8fbf8
            jmp     dword ptr [ee.x8fd30]

            jump_ee_x8fbf8:
            jmp     dword ptr [ee.x8fbf8]
        }
    }
    __declspec(naked) void __cdecl AudioFile_t::asm_func_if_speed_2() {
        __asm {
            cmp     dword ptr[ecx + 0x04], -0x64
            jg      jump_ee_x8fbf8
            jmp     dword ptr[ee.x8ffb1]

            jump_ee_x8fbf8:
            jmp     dword ptr[ee.x8fbf8]
        }
    }
    __declspec(naked) void __cdecl AudioFile_t::asm_func_speed_1() {
        __asm {
            fild    dword ptr [eax + 0x04]
            fimul   dword ptr [esp + 0x40]
            ret
        }
    }
    __declspec(naked) void __cdecl AudioFile_t::asm_func_speed_2() {
        __asm {
            fild    dword ptr [ecx + 0x04]
            fimul   dword ptr [esp + 0x40]
            ret
        }
    }

} // namespace patch
#endif // ifdef PATCH_SWITCH_OBJ_AUDIOFILE
