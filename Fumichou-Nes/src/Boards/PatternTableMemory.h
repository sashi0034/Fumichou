#pragma once

#include "Forward.h"

namespace Nes
{
	class PatternTableMemory
	{
	public:
		PatternTableMemory() = default;
		PatternTableMemory(std::span<const uint8> data);
		PatternTableMemory(size_t size);

		const s3d::Texture& Texture() const;

		uint8 Read8(addr16 addr) const { return m_bytes[addr]; }
		void Write8(addr16 addr, uint8 value);

	private:
		class Impl;

		s3d::Array<uint8> m_bytes{};

		struct Cache
		{
			s3d::Image image{};
			s3d::DynamicTexture texture{};
			bool refreshed{};
		} mutable m_cache;
	};
}
