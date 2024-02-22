#pragma once

#include "Forward.h"

namespace Nes
{
	// https://www.nesdev.org/wiki/PPU_registers
	// https://www.nesdev.org/wiki/PPU_scrolling

	// $2000
	class PpuControl8
	{
	public:
		PpuControl8(uint8 value = 0) : m_value(value) { return; }
		operator uint8() const { return m_value; }

		auto BaseNameTableAddrX() { return BitAccess<0>(m_value); }
		auto BaseNameTableAddrY() { return BitAccess<1>(m_value); }
		auto BaseNameTableAddrXY() { return BitAccess<0, 1>(m_value); }
		auto VramIncrementMode() { return BitAccess<2>(m_value); }
		auto SecondSprPatter() { return BitAccess<3>(m_value); }
		auto SecondBgPattern() { return BitAccess<4>(m_value); }
		auto LongSprite() { return BitAccess<5>(m_value); }
		auto Slave() { return BitAccess<6>(m_value); }
		auto NmiEnabled() { return BitAccess<7>(m_value); }

	private:
		uint8 m_value{};
	};

	// $2001
	class PpuMask8
	{
	public:
		PpuMask8(uint8 value = 0) : m_value(value) { return; }
		operator uint8() const { return m_value; }

	private:
		uint8 m_value{};
	};

	// $2002
	class PpuStatus8
	{
	public:
		PpuStatus8(uint8 value = 0) : m_value(value) { return; }
		operator uint8() const { return m_value; }

		// [0, 4] 未使用

		// スキャンラインで8スプライト以上検出されたか (ハードウェアバグあり)
		auto SprOverflow() { return BitAccess<5>(m_value); }

		// スプライト0とBGが不透明部分で重なったか
		auto SprZeroHit() { return BitAccess<6>(m_value); }

		// 垂直同期が始まったか
		auto VBlank() { return BitAccess<7>(m_value); }

	private:
		uint8 m_value{};
	};

	// $2006
	class PpuAddr16
	{
	public:
		PpuAddr16(uint16 value = 0) : m_value(value) { return; }
		operator uint16() const { return m_value; }

		auto CoarseX() { return BitAccess<0, 4>(m_value); }
		auto CoarseY() { return BitAccess<5, 9>(m_value); }
		auto NameTableAddr() { return BitAccess<10, 11>(m_value); }
		auto FineY() { return BitAccess<12, 14>(m_value); }
		// [15] Unused

	private:
		uint16 m_value{};
	};

	struct PpuRegs
	{
		PpuControl8 control; // $2000
		PpuMask8 mask; // $2001
		uint8 OamAddr; // $2003
		uint8 fineX;
		PpuAddr16 tempAddr;
	};

	struct PpuUnstableRegs
	{
		uint8 openBus;
		PpuStatus8 status; // $2002
		bool writeToggle;
		PpuAddr16 vramAddr; // $2006
	};

	struct OamData
	{
		uint8 y;
		uint8 tile;
		uint8 attribute;
		uint8 x;
	};

	static_assert(sizeof(std::array<OamData, 64>) == sizeof(std::array<uint8, 256>));

	class Ppu
	{
	public:
		class In;

		Ppu();

		const s3d::Texture& GetVideo() const;
		const std::array<addr16, 4>& GetNametableOffset() const { return m_nametableOffset; }
		const std::array<uint8, 4096>& GetNametableData() const { return m_nametableData; };

	private:
		class IRenderer;
		std::shared_ptr<IRenderer> m_renderer;

		NameTableMirror m_mirroring{};
		std::array<addr16, 4> m_nametableOffset{};
		std::array<uint8, 4096> m_nametableData{};
		std::array<uint8, 32> m_palettes{};

		union
		{
			std::array<OamData, 64> sprites;
			std::array<uint8, 256> bytes;
		} m_oam{};

		PpuCycle m_lineCycles{}; // [0, 341)
		uint32 m_scanLine{};
		bool m_scanningSprZero{};
		PpuRegs m_regs{};
		mutable PpuUnstableRegs m_unstable{};
	};
}
