#pragma once

#include "BoardBase.h"
#include "Ppu_In.h"

namespace Nes
{
	class Ppu::In::Renderer
	{
	public:
		struct render_args
		{
			std::reference_wrapper<Ppu> ppu;
			std::reference_wrapper<const Mmu> mmu;
			std::reference_wrapper<const BoardBase> board;
		};

		static void Render(const render_args& args);

	private:
		class Impl;
	};
}
