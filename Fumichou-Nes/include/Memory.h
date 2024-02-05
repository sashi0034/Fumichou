#pragma once

namespace Nes
{
	constexpr uint32 RamSize_0x800 = 0x800;

	// 内部RAM [0x0000, 0x07FF]
	class InternalRam : public std::array<uint8, RamSize_0x800>
	{
	};

	// カートリッジの外部RAM [0x8000, 0xFFFF] (ミラーあり?)
	class ExtraRam : public std::array<uint8, 0x2000>
	{
	};

	class Memory
	{
	public:
		InternalRam& GetInternalRam() { return m_internal; }
		const InternalRam& GetInternalRam() const { return m_internal; }

		ExtraRam& GetExternalRam() { return m_extraRam; }
		const ExtraRam& GetExternalRam() const { return m_extraRam; }

	private:
		InternalRam m_internal{};
		ExtraRam m_extraRam{};
	};
}
