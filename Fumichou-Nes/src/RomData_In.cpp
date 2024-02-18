#include "stdafx.h"
#include "RomData_In.h"

#include "Logger.h"

// https://www.nesdev.org/wiki/INES
// Header (16 bytes)
// Trainer, if present (0 or 512 bytes)
// PRG ROM data (16384 * x bytes)
// CHR ROM data, if present (8192 * y bytes)
// PlayChoice INST-ROM, if present (0 or 8192 bytes)
// PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)

namespace Nes
{
	bool RomData::In::LoadRomFile(RomData& self, s3d::FilePathView romPath)
	{
		s3d::BinaryReader reader{romPath};
		if (not reader.isOpen())
		{
			Logger::Error(fmt::format(U"Could not open {}", romPath));
			return false;
		}

		// ヘッダ16バイト読み込み
		std::array<uint8, 0x10> header{};
		reader.read(header.data(), 0x10);

		if (std::string{&header[0], &header[4]} != "NES\x1A")
		{
			Logger::Error(fmt::format(U"Invalid ROM header: {:02X} {:02X} {:02X} {:02X}",
			                          header[0], header[1], header[2], header[3]));
			return false;
		}

		if (GetBits<3>(header[6])) self.m_nameTableMirror = NameTableMirror::FourScreen;
		else self.m_nameTableMirror = GetBits<0>(header[6]) ? NameTableMirror::Horizontal : NameTableMirror::Vertical;

		self.m_mapperNumber = MapperNumber8(GetBits<4, 7>(header[6]) | (GetBits<4, 7>(header[7]) << 4));

		self.m_extendedRam = GetBits<1>(header[6]);

		if (GetBits<2>(header[6]))
		{
			Logger::Error(U"Trainer is not supported");
		}

		if (GetBits<0, 1>(header[10]) == 2 || GetBits<1>(header[10]))
		{
			Logger::Error(U"PAL ROM is not supported");
		}

		// PRG-ROM読み込み
		if (const uint8 prgBanks = header[4])
		{
			self.m_prgRom.resize(0x4000 * prgBanks);
			if (not reader.read(self.m_prgRom.data(), 0x4000 * prgBanks))
			{
				Logger::Error(U"Reading CHR-ROM failed.");
				return false;
			}
		}
		else
		{
			Logger::Error(U"ROM has no PRG-ROM banks.");
			return false;
		}

		// CHR-ROM読み込み
		if (const uint8 chrBanks = header[5])
		{
			self.m_chrRom.resize(0x2000 * chrBanks);
			if (not reader.read(self.m_chrRom.data(), 0x2000 * chrBanks))
			{
				Logger::Error(U"Reading CHR-ROM failed.");
				return false;
			}
		}

		return true;
	}
}
