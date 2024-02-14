#include "stdafx.h"
#include "PatternTableMemory.h"

#include "Logger.h"

using namespace Nes;

namespace
{
	constexpr int tileByteSize_0x10 = 0x10;
	constexpr int tilePixelSize_8 = 8;
}

class PatternTableMemory::Impl
{
public:
	static void Initialize(const PatternTableMemory& self)
	{
		const uint32 tilesCount = self.m_bytes.size() / tileByteSize_0x10;
		self.m_cache.image = Image(Size(tilesCount, 1) * tilePixelSize_8);
		self.m_cache.texture = DynamicTexture(self.m_cache.image, TextureFormat::R16G16_Unorm);
		if (self.m_cache.texture.isEmpty())
		{
			Logger::Abort(U"Failed to create Texture for Pattern Table");
		}
	}

	static void RefreshTexture(const PatternTableMemory& self)
	{
		const uint32 tilesCount = self.m_bytes.size() / tileByteSize_0x10;
		for (uint32 id = 0; id < tilesCount; ++id)
		{
			refreshTileImage(self, id);
		}

		// テクスチャに反映
		self.m_cache.texture.fill(self.m_cache.image);
	}

private:
	static void refreshTileImage(const PatternTableMemory& self, uint32 id)
	{
		const uint32 offsetU = id * tilePixelSize_8;
		const uint32 offsetBytes = id * tileByteSize_0x10;

		// タイル内容更新
		for (uint8 u = 0; u < tilePixelSize_8; ++u)
		{
			for (uint8 v = 0; v < tilePixelSize_8; ++v)
			{
				const bool bit0 = self.m_bytes[offsetBytes + v + 0x0] & (1 << (7 - u));
				const bool bit1 = self.m_bytes[offsetBytes + v + 0x8] & (1 << (7 - u));
				auto& pixel = self.m_cache.image[Point(offsetU + u, v)];
				pixel.setR(bit0 * 0xFF);
				pixel.setG(bit1 * 0xFF);
			}
		}
	}
};

namespace Nes
{
	PatternTableMemory::PatternTableMemory(std::span<const uint8> data):
		m_bytes(data.begin(), data.end())
	{
		Impl::Initialize(*this);
	}

	PatternTableMemory::PatternTableMemory(size_t size)
	{
		m_bytes.resize(size);
		Impl::Initialize(*this);
	}

	const Texture& PatternTableMemory::Texture() const
	{
		if (not m_cache.refreshed)
		{
			// キャッシュが古くなったので更新
			Impl::RefreshTexture(*this);
			m_cache.refreshed = true;
		}
		return m_cache.texture;
	}

	void PatternTableMemory::Write8(addr16 addr, uint8 value)
	{
		m_bytes[addr] = value;
		m_cache.refreshed = false;
		// TODO: 更新対象をメモ
	}
}
