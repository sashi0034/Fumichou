#pragma once
#include "IMapper.h"
#include "RomData.h"

namespace Nes
{
	class Cartridge
	{
	public:
		class In;

	private:
		RomData m_romData{};
		std::unique_ptr<IMapper> m_mapper{};
	};
}
