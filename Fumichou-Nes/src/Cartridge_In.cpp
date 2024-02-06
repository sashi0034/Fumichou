#include "stdafx.h"
#include "Cartridge_In.h"

#include "Logger.h"
#include "RomData_In.h"
#include "Mappers/MapperEmpty.h"
#include "Mappers/MapperNrom.h"

struct Nes::Cartridge::In::Impl
{
	static bool LoadRomFile(Cartridge& self, FilePathView romPath)
	{
		if (not RomData::In::LoadRomFile(self.m_romData, romPath)) return false;

		self.m_mapper = createMapper(self);
		self.m_mapper->Initialize(self.m_romData);

		return true;
	}

private:
	[[nodiscard]]
	static std::unique_ptr<IMapper> createMapper(const Cartridge& self)
	{
		// https://www.nesdev.org/wiki/Mapper
		switch (const auto mapper = self.m_romData.GetMapperNumber())
		{
		case 0:
			return std::make_unique<MapperNrom>();

		default:
			Logger::Error(fmt::format("Unsupported mapper: {}", mapper));
			return std::make_unique<MapperEmpty>();
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
