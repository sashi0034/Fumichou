#include "stdafx.h"
#include "HwFrame.h"

#include "Cartridge.h"
#include "Cartridge_In.h"

struct Nes::HwFrame::Impl
{
	Hardware m_hardware{};
};

namespace Nes
{
	HwFrame::HwFrame() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void HwFrame::LoadRomFile(FilePathView romPath)
	{
		Cartridge::In::LoadRomFile(p_impl->m_hardware.GetCartridge(), romPath);
	}

	const Hardware& HwFrame::GetEnv()
	{
		return p_impl->m_hardware;
	}
}
