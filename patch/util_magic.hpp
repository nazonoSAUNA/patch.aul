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
#include <cstdint>
#include <bit>
#include <algorithm>

#include <Windows.h>

#include "global.hpp"

#include "util_int.hpp"
#include "util_pe.hpp"


inline i32 CalcNearJmp(i32 address, i32 jmp_address) {
	return jmp_address - (address + 4);
}


class OverWriteOnProtectHelper {
	uintptr_t m_address, m_size;
	DWORD m_oldProtect;
public:
	template<class T>
	OverWriteOnProtectHelper(T address, uintptr_t size) noexcept : m_address((uintptr_t)address), m_size(size) {
		VirtualProtect(reinterpret_cast<LPVOID>(m_address), m_size, PAGE_EXECUTE_READWRITE, &m_oldProtect);
	}
	~OverWriteOnProtectHelper() noexcept {
		VirtualProtect(reinterpret_cast<LPVOID>(m_address), m_size, m_oldProtect, &m_oldProtect);
	}

	template<class T1, class T2>
	void store_i8(T1 address, T2 value) const {
		::store_i8(m_address + address, value);
	}
	template<class T1, class T2>
	void store_i16(T1 address, T2 value) const {
		::store_i16(m_address + address, value);
	}
	template<class T1, class T2>
	void store_i32(T1 address, T2 value) const {
		::store_i32(m_address + address, value);
	}
	template<class T1, class T2>
	void store_i64(T1 address, T2 value) const {
		::store_i64(m_address + address, value);
	}
	template<class T1, class T2>
	void store_rel32(T1 address, T2 value) const {
		::store_rel32(m_address + address, value);
	}

	template<class T0 = uint8_t, class T1>
	T0 load_i8(T1 address) const {
		return ::load_i8<T0>(m_address + address);
	}
	template<class T0 = uint16_t, class T1>
	T0 load_i16(T1 address) const {
		return ::load_i16<T0>(m_address + address);
	}
	template<class T0 = uint32_t, class T1>
	T0 load_i32(T1 address) const {
		return ::load_i32<T0>(m_address + address);
	}
	template<class T0 = uint64_t, class T1>
	T0 load_i64(T1 address) const {
		return ::load_i64<T0>(m_address + address);
	}
	template<class T0 = uint32_t, class T1>
	T0 load_rel32(T1 address) const {
		return ::load_rel32<T0>(m_address + address);
	}

	inline void replaceNearJmp(i32 offset, void* jmp_address) {
		store_rel32(offset, jmp_address);
	}

	auto address() const {
		return m_address;
	}
	auto address(uintptr_t ofs) const {
		return m_address + ofs;
	}
};

/// <summary>
/// ニアージャンプ・コールを書き換える
/// </summary>
/// <param name="address">書き換える対象のアドレス</param>
/// <param name="jmp_address">代わりに飛ばして欲しいアドレス</param>
inline void ReplaceNearJmp(i32 address, void* jmp_address) {
	OverWriteOnProtectHelper(address, 4).replaceNearJmp(0, jmp_address);
}

// 既存の関数を破壊して，自分の関数を実行する
inline class ReplaceFunction_t {
	static const int asm_size = 5;

public:
	// 乗っ取りたい関数があるアドレス,ジャンプさせる関数のポインタ,元の関数の内容が返る場所
	template<class T, size_t N = asm_size, std::enable_if_t<N >= asm_size, std::nullptr_t> = nullptr>
	void operator()(T address, const void* function, std::byte(&original)[N]) noexcept {
		auto adr = std::bit_cast<i32>(address);
		OverWriteOnProtectHelper h(adr, asm_size);
		std::copy(adr, adr + N, original);
		store_i8(adr, '\xe9'); // jmp rel32
		store_i32(adr + 1, CalcNearJmp(address + 1, (i32)function));
	}

	template<class T>
	void operator()(T address, const void* function) noexcept {
		auto adr = std::bit_cast<i32>(address);
		OverWriteOnProtectHelper h(adr, asm_size);
		store_i8(adr, '\xe9'); // jmp rel32
		store_rel32(adr + 1, function);
	}
} ReplaceFunction;


// 乗っ取りたいモジュール, 乗っ取る関数があるDLLのファイル名, 乗っ取る関数の名前, 新しい関数へのポインタ
inline BOOL ExchangeFunction(HMODULE hModule, std::string_view modname, std::string_view funcname, void* function) noexcept {
	auto ptr = search_import(hModule, modname, funcname);
	if (!ptr)return FALSE;
	DWORD flOldProtect;
	if (VirtualProtect(ptr, 4, PAGE_EXECUTE_READWRITE, &flOldProtect) == FALSE) return FALSE;
	store_i32(ptr, function);
	return VirtualProtect(ptr, 4, flOldProtect, &flOldProtect);
}

/// <summary>
/// 指定したアドレスの関数の直前に、自分の関数を実行する
/// 実行後元の関数に戻る
/// __stdcall,__cdecl専用
/// </summary>
/// <param name="address"> 中断したい関数のアドレス </param>
/// <param name="function"> 挿入する関数 </param>
/// <param name="asm_word_n"> 命令単位に合った数(5以上) </param>
/// <returns> TRUE </returns>
inline bool InjectFunction_stdcall(uint32_t address, const void* func, size_t asm_word_n) noexcept {
	if (asm_word_n < 5)return false;

	auto& cursor = GLOBAL::executable_memory_cursor;
	auto bridge = GLOBAL::executable_memory_cursor;

	store_i8(cursor, '\xe8'); cursor++; // CALL (rel32)
	store_rel32(cursor, func); cursor += 4;
	std::copy((std::byte*)address, (std::byte*)address + asm_word_n, cursor); cursor += asm_word_n;
	store_i8(cursor, '\xe9'); cursor++; // JMP (rel32)
	store_rel32(cursor, address + asm_word_n); cursor += 4;

	OverWriteOnProtectHelper h(address, asm_word_n);
	h.store_i8(0, '\xe9'); // JMP (rel32)
	h.store_rel32(1, bridge);

	return true;
}

/// <summary>
/// 指定したアドレスの関数の直前に、自分の関数を実行する
/// 実行後元の関数に戻る
/// __cdecl専用 (__stdcallと一緒だけど)
/// </summary>
/// <param name="address"> 中断したい関数のアドレス </param>
/// <param name="function"> 挿入する関数 </param>
/// <param name="asm_word_n"> 命令単位に合った数(5以上) </param>
/// <returns> TRUE </returns>
inline bool InjectFunction_cdecl(uint32_t address, const void* function, size_t asm_word_n) noexcept {
	InjectFunction_stdcall(address, function, asm_word_n);
}

/// <summary>
/// 指定したアドレスの関数の直前に、自分の関数を実行する
/// 実行後元の関数に戻る
/// __fastcall専用
/// </summary>
/// <param name="address"> 中断したい関数のアドレス </param>
/// <param name="function"> 挿入する関数 </param>
/// <param name="asm_word_n"> 命令単位に合った数(5以上) </param>
/// <returns> TRUE </returns>
inline bool InjectFunction_fastcall(uint32_t address, void(*func)(), size_t asm_word_n) {
	if (asm_word_n < 5)return false;

	auto& cursor = GLOBAL::executable_memory_cursor;
	auto bridge = GLOBAL::executable_memory_cursor;

	store_i32(cursor, '\x51\x52\xe8\x00'); cursor += 3; // PUSH ECX; PUSH EDX; CALL (rel32)
	store_rel32(cursor, func); cursor += 4;
	store_i16(cursor, '\x5a\x59'); cursor += 2; // POP EDX; POP ECX
	std::copy((std::byte*)address, (std::byte*)address + asm_word_n, cursor); cursor += asm_word_n;
	store_i8(cursor, '\xe9'); cursor++; // JMP (rel32)
	store_rel32(cursor, address + asm_word_n); cursor += 4;

	OverWriteOnProtectHelper h(address, asm_word_n);
	h.store_i8(0, '\xe9'); // JMP (rel32)
	h.store_rel32(1, bridge);

	return true;
}



inline static intptr_t __cdecl repush_args_call(void* function, int arg_ofs, int arg_n);

/// <summary>
/// 指定したアドレスにて、自分の関数を実行する
/// 実行後、元のアドレスに戻る
/// __cdecl専用
/// </summary>
/// <param name="address"> 中断したいアドレス </param>
/// <param name="function"> 挿入する関数 </param>
/// <param name="asm_word_n"> 命令単位に合った数(5以上) </param>
/// <param name="stack_s"> 何番目のスタックから引数にするか(4byte単位) </param>
/// <param name="arg_n"> 引数の数(4byte単位) </param>
/// <param name="flag"> EAX,ECX,EDXを退避するかを設定 </param>
/// <returns> TRUE </returns>
#define FLAG_PUSH_POP_EAX 0x00000001 // 戻り値が元のEAXに上書きされる注意
#define FLAG_PUSH_POP_ECX 0x00000002
#define FLAG_PUSH_POP_EDX 0x00000004
// それ以外は呼出規約として関数側が行っている
inline void InjectionFunction_push_args_cdecl(uint32_t address, const uint32_t function, size_t asm_word_n, uint16_t stack_s, uint8_t arg_n, uint32_t flag) {
	auto& cursor = GLOBAL::executable_memory_cursor;
	auto bridge = GLOBAL::executable_memory_cursor;
	stack_s++; // バイナリ関数にpush ebpがある分
	// push
	if (flag & FLAG_PUSH_POP_EAX) {
		store_i8(cursor, 0x50); cursor++;
		stack_s++;
	}
	if (flag & FLAG_PUSH_POP_ECX) {
		store_i8(cursor, 0x51); cursor++;
		stack_s++;
	}
	if (flag & FLAG_PUSH_POP_EDX) {
		store_i8(cursor, 0x52); cursor++;
		stack_s++;
	}

	// args eax,ecx,edx
	store_i8(cursor, 0xb8); cursor++; // mov eax, function
	store_i32(cursor, function); cursor += 4;
	store_i8(cursor, 0xb9); cursor++; // mov ecx, arg_n
	store_i32(cursor, arg_n); cursor += 4;
	store_i8(cursor, 0xba); cursor++; // mov edx, stack_s
	store_i32(cursor, stack_s); cursor += 4;
	store_i8(cursor, 0xe8); cursor++;
	store_rel32(cursor, &repush_args_call); cursor += 4;

	// pop
	if (flag & FLAG_PUSH_POP_EDX) {
		store_i8(cursor, 0x5a); cursor++;
	}
	if (flag & FLAG_PUSH_POP_ECX) {
		store_i8(cursor, 0x59); cursor++;
	}
	if (flag & FLAG_PUSH_POP_EAX) {
		store_i8(cursor, 0x58); cursor++;
	}

	memcpy(cursor, (void*)address, asm_word_n); cursor += asm_word_n;

	store_i8(cursor, 0xe9); cursor++;
	store_rel32(cursor, address + asm_word_n); cursor += 4;


	OverWriteOnProtectHelper h(address, 5);
	h.store_i8(0, 0xe9);
	h.replaceNearJmp(1, bridge);
}

inline __declspec(naked) intptr_t repush_args_call(void* eax_function, int ecx_arg_n, int edx_arg_of) {
	__asm {
		push	ebp
		mov		ebp, esp
		add		edx, ecx
		lea		edx, [esp + edx * 4]
		test	ecx, ecx
		jle		skip
			back:
				push	dword ptr [edx]
				sub		edx, 4
			loop	back
		skip:
		
		call	eax
		mov		esp, ebp
		pop		ebp
		ret
	}
}


/**
 * @brief __fastcallのfuncを実行する。スタック除去のタイミングが問題にならないように実行できる
 *
 * @param arg_n funcの引数の数（最小3）
 * @param func __fastcallの関数のポインタ
 * @param arg0 1番目の引数
 * @param arg1 2番目の引数
 * @param arg2 3番目の引数
 * @param ... 4番目以降の引数
 * @return EAXでそのまま返す
 *
 * @note 引数が2個以下の場合はこの関数を経由する意味は無いため出来ないようにしました。func(arg0, arg1)で呼び出してください
 * @warning 浮動小数や非32bitの引数は考慮していない
 */
inline __declspec(naked) intptr_t __cdecl fastcall_caller(int arg_n, void* func, intptr_t arg0, intptr_t arg1, intptr_t arg2, ...) {
	__asm {
		mov edx, ebp
		mov ecx, dword ptr [esp + 0x04]
		lea ebp, dword ptr [esp - 0x0c]
		add ecx, 0x03
		SHIFT_STACK:
			mov eax, dword ptr [ebp + 0x0c]
			mov dword ptr [ebp], eax
			add ebp, 0x04
		loop SHIFT_STACK
		mov dword ptr [ebp], edx
		mov dword ptr [ebp + 0x04], esp
		mov eax, dword ptr [esp - 0x0c]
		mov dword ptr [ebp + 0x08], eax
		mov edx, dword ptr [esp + 0x04]
		mov ecx, dword ptr [esp]
		add esp, 0x08
		call dword ptr [esp - 0x0c]
		mov ecx, dword ptr [ebp + 0x08]
		mov esp, dword ptr [ebp + 0x04]
		mov dword ptr [esp], ecx
		mov ebp, dword ptr [ebp]
		ret
	}
}
