#pragma once
#include "Util.h"

namespace Nes
{
	using namespace Util;

	using s3d::uint8;
	using s3d::uint16;
	using s3d::uint32;
	using s3d::uint64;

	using sint8 = s3d::int8;
	using sint16 = s3d::int16;
	using sint32 = s3d::int32;
	using sint64 = s3d::int64;

	using s3d::String;
	using s3d::StringView;
	using s3d::Literals::StringViewLiterals::operator ""_sv;
	using s3d::Literals::FormatLiterals::operator ""_fmt;

	using s3d::Range;

	using addr16 = uint16;

	using CpuCycle = int;
	using PpuCycle = int;

	class Hardware;

	constexpr uint32 AddrSize_0x10000 = 0x10000;

	constexpr uint32 DisplayWidth_256 = 256;
	constexpr uint32 DisplayHeight_240 = 240;
	constexpr s3d::Size Display_256x240{DisplayWidth_256, DisplayHeight_240};

	constexpr uint32_t SampleRate_43653 = 43653;

	// https://www.nesdev.org/wiki/Mirroring#Nametable_Mirroring
	enum class NameTableMirror
	{
		Horizontal,
		Vertical,
		SingleLower,
		SingleHigher,
		FourScreen,
	};

	class EmulationAbort final : public s3d::Error
	{
	public:
		using Error::Error;
		[[nodiscard]] StringView type() const noexcept override { return U"EmulationAbort"; }
	};
}
