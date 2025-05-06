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
#include <algorithm>

// 6%弱のパターンで+1か-1の誤差があるため精度が重要な場面では使わないこと

class FastDiv {
private:
	uint32_t multiplier;
	int32_t divisor;
public:
	FastDiv(int32_t divisor) {
		set_divisor(divisor);
	}
	static inline uint32_t mulhi(uint32_t a, uint32_t b) {
		return static_cast<uint32_t>((static_cast<uint64_t>(a) * b) >> 32);
	}
	inline int32_t div(int32_t n) {
		uint32_t abs_n = abs(n);
		int32_t r;
		if (multiplier == 0) {
			r = abs_n;
		} else {
			r = mulhi(abs_n, multiplier);
		}
		if ((divisor ^ n) < 0) {
			r = -r;
		}
		return r;
	}
	inline void set_divisor(int32_t divisor) {
		this->divisor = divisor;
		uint32_t abs_d = abs(divisor);
		if (abs_d <= 1) {
			multiplier = 0;
			return;
		}
		uint32_t m = static_cast<uint32_t>((0x100000000ULL + (abs_d >> 1)) / abs_d);
		multiplier = m;
	}
	inline int32_t get_divisor() {
		return this->divisor;
	}
};

class FastDiv16 {
private:
	uint16_t multiplier;
	int16_t divisor;
public:
	FastDiv16(int16_t divisor) {
		set_divisor(divisor);
	}
	static inline uint16_t abs16(int16_t n) {
		int16_t mask = n >> 15;
		return (n ^ mask) - mask;
	}
	static inline uint16_t mulhi16(uint16_t a, uint16_t b) {
		return static_cast<uint16_t>((static_cast<uint32_t>(a) * b) >> 16);
	}
	inline int16_t div(int16_t n) {
		uint16_t abs_n = abs16(n);
		int16_t r;
		if (multiplier == 0) {
			r = abs_n;
		} else {
			r = mulhi16(abs_n, multiplier);
		}
		if ((divisor ^ n) < 0) {
			r = -r;
		}
		return r;
	}
	inline void set_divisor(int16_t divisor) {
		this->divisor = divisor;
		uint16_t abs_d = abs16(divisor);
		if (abs_d <= 1) {
			multiplier = 0;
			return;
		}
		uint16_t m = static_cast<uint16_t>((0x10000 + (abs_d >> 1)) / abs_d);
		multiplier = m;
	}
	inline int16_t get_divisor() {
		return this->divisor;
	}
};



/*
		constexpr int nnn = 2000000000;
		constexpr int step = 1 << 23;
		constexpr int r = step - 1;
		int cnt_total = 0;
		int cnt_n = 0;
		for (int y = 1; y < nnn; y += step) {
			int yy = y + (rand() & r);
			FastDiv fd(yy);
			for (int x = -nnn; x < nnn; x += step) {
				cnt_total++;
				int xx = x + (rand() & r);
				if (xx / yy != fd.div(xx)) {
					cnt_n++;
					printf("%d/%d, /=%d, fd=%d\n", xx, yy, xx / yy, fd.div(xx));
				}
			}
		}
		for (int y = -nnn; y < step; y += step) {
			int yy = y + (rand() & r);
			FastDiv fd(yy);
			for (int x = -nnn; x < nnn; x += step) {
				cnt_total++;
				int xx = x + (rand() & r);
				if (xx / yy != fd.div(xx)) {
					cnt_n++;
					printf("%d/%d, /=%d, fd=%d\n", xx, yy, xx / yy, fd.div(xx));
				}
			}
		}
		printf("%d/%d", cnt_n, cnt_total);

*/