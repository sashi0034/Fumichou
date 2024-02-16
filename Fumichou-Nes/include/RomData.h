#pragma once

#include "Forward.h"

namespace Nes
{
	class MapperNumber8 : public EnumValue<uint8>
	{
		using EnumValue::EnumValue;
	};

	class PrgRom : public Array<uint8>
	{
	};

	class ChrRom : public Array<uint8>
	{
	};

	class RomData
	{
	public:
		const PrgRom& GetPrg() const { return m_prgRom; }
		const ChrRom& GetChr() const { return m_chrRom; }
		MapperNumber8 GetMapperNumber() const { return m_mapperNumber; }
		NameTableMirror GetNameTableMirror() const { return m_nameTableMirror; }

		class In;

	private:
		PrgRom m_prgRom{};
		ChrRom m_chrRom{};
		NameTableMirror m_nameTableMirror{};
		MapperNumber8 m_mapperNumber{};
		bool m_extendedRam{};
	};
}
