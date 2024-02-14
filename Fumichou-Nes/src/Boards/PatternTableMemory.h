﻿#pragma once

#include "Forward.h"

namespace Nes
{
	class PatternTableMemory
	{
	public:
		PatternTableMemory() = default;
		PatternTableMemory(std::span<const uint8> data);
		PatternTableMemory(size_t size);

		const Texture& Texture() const;

		uint8 Read8(addr16 addr) const { return m_bytes[addr]; }
		void Write8(addr16 addr, uint8 value);

	private:
		class Impl;

		Array<uint8> m_bytes{};

		struct Cache
		{
			Image image{};
			DynamicTexture texture{};
			bool refreshed{};
		} mutable m_cache;
	};
}