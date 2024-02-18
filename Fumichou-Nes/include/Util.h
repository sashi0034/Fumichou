#pragma once

namespace Nes::Util
{
	template <class...>
	constexpr std::false_type AlwaysFalse{};

	template <int x1, int x2 = x1, typename T>
	T GetBits(T value)
	{
		static_assert(std::is_integral<T>::value);
		static_assert(x1 <= x2 && x2 < std::numeric_limits<T>::digits);

		if constexpr (x2 + 1 == std::numeric_limits<T>::digits)
		{
			return value >> x1;
		}

		constexpr T mask = ((T(1) << (x2 - x1 + 1)) - 1) << x1;
		return (value & mask) >> x1;
	}

	template <int x1, int x2 = x1, typename T, typename U>
	[[nodiscard]] T SetBits(T value, U newBits)
	{
		static_assert(std::is_integral<T>::value);
		static_assert(std::is_convertible<U, T>::value);
		static_assert(x1 <= x2 && x2 < std::numeric_limits<T>::digits);

		constexpr T mask1 = ((T(1) << (x2 - x1 + 1)) - 1);
		const T newBits1 = static_cast<U>(newBits) & mask1;

		constexpr T mask2 = mask1 << x1;
		return (value & ~mask2) | ((newBits1 << x1) & mask2);
	}

	template <int x1, int x2, typename T>
	class BitAccessor
	{
		static_assert(std::is_integral<T>::value);
		static_assert(x1 <= x2 && x2 < std::numeric_limits<T>::digits);

	public:
		explicit BitAccessor(T& ref) : m_ref(ref) { return; };
		T Get() const { return GetBits<x1, x2>(m_ref); }
		operator T() const { return Get(); }
		void Set(T value) { m_ref = SetBits<x1, x2>(m_ref, value); }

	private:
		T& m_ref;
	};

	template <int x1, int x2 = x1, typename T>
	auto BitAccess(T& ref)
	{
		return BitAccessor<x1, x2, T>(ref);
	}

	template <typename Wire>
	inline Wire ReadBytes(std::span<const uint8_t> span, uint64_t offset)
	{
		static_assert(std::endian::native == std::endian::little);
		static_assert(std::is_integral<Wire>::value);
		if constexpr (sizeof(Wire) == 8) // 64-bit
		{
			const uint32_t hi = *reinterpret_cast<const uint32_t*>(&span[offset + 0]);
			const uint32_t lo = *reinterpret_cast<const uint32_t*>(&span[offset + 4]);
			Wire result = (static_cast<Wire>(hi) << 32) | static_cast<Wire>(lo);
			return result;
		}
		else
		{
			return *reinterpret_cast<const Wire*>(&span[offset]);
		}
	}

	inline uint64_t ReadBytes64(std::span<const uint8_t> span, uint64_t offset)
	{
		return ReadBytes<uint64_t>(span, offset);
	}

	inline uint32_t ReadBytes32(std::span<const uint8_t> span, uint64_t offset)
	{
		return ReadBytes<uint32_t>(span, offset);
	}

	inline uint16_t ReadBytes16(std::span<const uint8_t> span, uint64_t offset)
	{
		return ReadBytes<uint16_t>(span, offset);
	}

	inline uint8_t ReadBytes8(std::span<const uint8_t> span, uint64_t offset)
	{
		return ReadBytes<uint8_t>(span, offset);
	}

	template <typename Wire>
	inline void WriteBytes(std::span<uint8_t> span, uint64_t offset, Wire value)
	{
		static_assert(std::endian::native == std::endian::little);
		static_assert(std::is_integral<Wire>::value);
		if constexpr (sizeof(Wire) == 8) // 64-bit
		{
			const uint32_t hi = value >> 32;
			const uint32_t lo = value;

			*reinterpret_cast<uint32_t*>(&span[offset + 0]) = hi;
			*reinterpret_cast<uint32_t*>(&span[offset + 4]) = lo;
		}
		else
		{
			*reinterpret_cast<Wire*>(&span[offset]) = value;
		}
	}

	inline void WriteBytes64(std::span<uint8_t> span, uint64_t offset, uint64_t value)
	{
		WriteBytes<uint64_t>(span, offset, value);
	}

	inline void WriteBytes32(std::span<uint8_t> span, uint64_t offset, uint32_t value)
	{
		WriteBytes<uint32_t>(span, offset, value);
	}

	inline void WriteBytes16(std::span<uint8_t> span, uint64_t offset, uint16_t value)
	{
		WriteBytes<uint16_t>(span, offset, value);
	}

	inline void WriteBytes8(std::span<uint8_t> span, uint64_t offset, uint8_t value)
	{
		WriteBytes<uint8_t>(span, offset, value);
	}

	template <typename T>
	class EnumValue
	{
		static_assert(std::is_integral<T>::value);

	public:
		explicit constexpr EnumValue() : value{0} { return; }
		explicit constexpr EnumValue(T v) : value{v} { return; }
		constexpr operator T() const { return value; }

	private:
		T value;
	};

	template <typename T = uint32_t>
	class AddrRange
	{
		static_assert(std::is_integral<T>::value);

	public:
		explicit consteval AddrRange(T baseAddr, T endAddr) : m_base{baseAddr}, m_end{endAddr}
		{
			if (baseAddr >= endAddr) throw std::logic_error("Base should be less than end.");
		}

		bool IsBetween(T addr) const { return m_base <= addr && addr <= m_end; }

		constexpr T base() const { return m_base; }
		constexpr T end() const { return m_end; }

	private:
		T m_base;
		T m_end;
	};
}
