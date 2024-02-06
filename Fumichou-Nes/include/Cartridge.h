#pragma once
#include "BoardBase.h"

namespace Nes
{
	class Cartridge
	{
	public:
		class In;

	private:
		std::unique_ptr<BoardBase> m_mapper{};
	};
}
