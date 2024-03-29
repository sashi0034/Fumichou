﻿#include "stdafx.h"
#include "Cartridge_In.h"

#include "Logger.h"
#include "RomData_In.h"
#include "Boards/BoardCnrom.h"
#include "Boards/BoardNrom.h"

struct Nes::Cartridge::In::Impl
{
	static bool LoadRomFile(Cartridge& self, s3d::FilePathView romPath)
	{
		auto rom = RomData();
		if (not RomData::In::LoadRomFile(rom, romPath)) return false;

		self.m_board = createBoard(rom);
		self.m_board->Initialize();

		if (not self.m_board) return false;

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
		case 3:
			return CreateBoardCnrom(rom);

		default:
			Logger::Error(fmt::format(U"Unsupported mapper: {}", static_cast<uint8>(mapper)));
			return CreateBoardNrom(rom);
		}
	}
};

namespace Nes
{
	bool Cartridge::In::LoadRomFile(Cartridge& self, s3d::FilePathView romPath)
	{
		return Impl::LoadRomFile(self, romPath);
	}
}
