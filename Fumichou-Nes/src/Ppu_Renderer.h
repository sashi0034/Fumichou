#pragma once

#include "Ppu.h"

namespace Nes
{
	class BoardBase;
	class Mmu;

	class Ppu::IRenderer
	{
	public:
		virtual ~IRenderer() = default;
		static std::unique_ptr<IRenderer> Create();

		struct render_args
		{
			std::reference_wrapper<const Ppu> ppu;
			std::reference_wrapper<const Mmu> mmu;
			std::reference_wrapper<const BoardBase> board;
		};

		virtual const s3d::Texture& GetVideoTexture() const = 0;
		virtual void Render(const render_args& args) = 0;

	private:
		class Hle;
	};
}
