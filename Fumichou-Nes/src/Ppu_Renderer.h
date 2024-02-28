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

		virtual void Render(const render_args& args) = 0;

		virtual const s3d::Texture& GetVideoTexture() const = 0;
		virtual void SetScrollX(uint32 scanline, uint16 scrollX) = 0;
		virtual void SetScrollY(uint32 scrollY) = 0; // FIXME: ゼルダなどは途中でYを変えていそうなので対策
		virtual uint32 GetScrollY() const = 0;
		virtual const std::array<OamData, 64>& GetRenderedSprites() const = 0;

	private:
		class HleRenderer;
	};
}
