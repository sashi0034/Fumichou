#pragma once

#include "Mmu.h"
#include "RomData.h"

namespace Nes
{
	class BoardBase
	{
	public:
		BoardBase(const RomData& rom): m_rom(std::move(rom)) { return; }
		const RomData& GetRom() { return m_rom; }

		virtual ~BoardBase() = default;

		virtual void Initialize() = 0;

		[[nodiscard]] virtual MappedRead MapReadPrg(addr16 addr) const = 0;
		[[nodiscard]] virtual MappedWrite MapWritePrg(addr16 addr) = 0;

		[[nodiscard]] virtual MappedRead MapReadChr(addr16 addr) const = 0;
		[[nodiscard]] virtual MappedWrite MapWriteChr(addr16 addr) = 0;

		// 64個区切りでタイルのオフセットを表現 (バンクを持つマッパーはタイルのオフセットが変わる)
		[[nodiscard]] virtual std::array<uint16, 8> TilePageOffsets() const = 0;

		// 描画で扱いやすいようにあらかじめパターンテーブルはデコードした状態にする
		[[nodiscard]] virtual const s3d::Image& PatternTableImage() const = 0;
		[[nodiscard]] virtual const s3d::Texture& PatternTableTexture() const = 0;

	protected:
		RomData m_rom{};
	};
}
