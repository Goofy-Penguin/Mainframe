#pragma once

namespace mainframe {
	namespace ui {
		enum class ModifierKey {
			none = 0,
			shift = 1,
			ctrl = 2
		};

		inline constexpr ModifierKey operator&(ModifierKey x, ModifierKey y) {
			return static_cast<ModifierKey>(static_cast<int>(x)& static_cast<int>(y));
		}

		inline constexpr ModifierKey operator|(ModifierKey x, ModifierKey y) {
			return static_cast<ModifierKey>(static_cast<int>(x) | static_cast<int>(y));
		}

		inline constexpr ModifierKey operator^(ModifierKey x, ModifierKey y) {
			return static_cast<ModifierKey>(static_cast<int>(x) ^ static_cast<int>(y));
		}

		inline constexpr ModifierKey operator~(ModifierKey x) {
			return static_cast<ModifierKey>(~static_cast<int>(x));
		}

		inline ModifierKey& operator&=(ModifierKey& x, ModifierKey y) {
			x = x & y;
			return x;
		}

		inline ModifierKey& operator|=(ModifierKey& x, ModifierKey y) {
			x = x | y;
			return x;
		}

		inline ModifierKey& operator^=(ModifierKey& x, ModifierKey y) {
			x = x ^ y;
			return x;
		}

		inline constexpr bool operator>=(ModifierKey& x, unsigned int y) {
			return static_cast<unsigned int>(x) >= y;
		}

		inline constexpr bool operator>(ModifierKey& x, unsigned int y) {
			return static_cast<unsigned int>(x) > y;
		}

		inline constexpr bool operator<=(ModifierKey& x, unsigned int y) {
			return static_cast<unsigned int>(x) >= y;
		}

		inline constexpr bool operator<(ModifierKey& x, unsigned int y) {
			return static_cast<unsigned int>(x) > y;
		}
	}
}