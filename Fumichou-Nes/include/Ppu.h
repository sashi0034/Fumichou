#pragma once

#include "Forward.h"

namespace Nes
{
	// https://www.nesdev.org/wiki/PPU_registers

	// $2000
	class PpuControl8
	{
	public:
		PpuControl8(uint8 value) : m_value(value) { return; }
		operator uint8() const { return m_value; }

		auto BaseNameTableAddr() { return BitAccess<0, 1>(m_value); }
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
		PpuMask8(uint8 value) : m_value(value) { return; }
		operator uint8() const { return m_value; }

	private:
		uint8 m_value{};
	};

	// $2002
	class PpuStatus8
	{
	public:
		PpuStatus8(uint8 value) : m_value(value) { return; }
		operator uint8() const { return m_value; }

	private:
		uint8 m_value{};
	};

	struct PpuRegs
	{
		PpuControl8 control; // $2000
		PpuMask8 mask; // $2001
		PpuStatus8 status; // $2002
		uint8 OamOffset; // $2003
	};

	class Ppu
	{
	public:
		class In;

	private:
		PpuCycle m_lineCycles{}; // [0, 341)
		uint32 m_scanLine{};
	};
}
