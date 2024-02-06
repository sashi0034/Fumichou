#include "stdafx.h"
#include "Cartridge_In.h"

#include "Logger.h"
#include "RomData_In.h"
#include "Boards/BoardNrom.h"

struct Nes::Cartridge::In::Impl
{
	static bool LoadRomFile(Cartridge& self, FilePathView romPath)
	{
		auto rom = RomData();
		if (not RomData::In::LoadRomFile(rom, romPath)) return false;

		self.m_mapper = createBoard(rom);
		self.m_mapper->Initialize();

		if (not self.m_mapper) return false;

		return true;
	}

private:
	[[nodiscard]]
	static std::unique_ptr<BoardBase> createBoard(const RomData& rom)
	{
		// https://www.nesdev.org/wiki/Mapper
		switch (const auto mapper = rom.GetMapperNumber())
		{
		case 0:
			return CreateBoardNrom(rom);

		default:
			Logger::Error(fmt::format("Unsupported mapper: {}", mapper));
			return CreateBoardNrom(rom);
		}
	}
};

namespace Nes
{
	bool Cartridge::In::LoadRomFile(Cartridge& self, FilePathView romPath)
	{
		return Impl::LoadRomFile(self, romPath);
	}
}
