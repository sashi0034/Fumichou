﻿#include "stdafx.h"
#include "HwFrame.h"

#include "Cartridge.h"
#include "Cartridge_In.h"
#include "HardwareConstants.h"
#include "Mmu_In.h"
#include "Mos6502_In.h"

struct Nes::HwFrame::Impl
{
	Hardware m_hardware{};
	Optional<EmulationAbort> m_abort{none};

	bool StartRomFile(FilePathView romPath)
	{
		if (not Cartridge::In::LoadRomFile(m_hardware.GetCartridge(), romPath))
		{
			return false;
		}

		Mmu::In::MapWholeAddr(m_hardware);

		Mos6502::In::Reset(m_hardware);

		return true;
	}

	void ControlFrames()
	{
		if (m_abort.has_value()) return;

		// TODO
		try
		{
			emulateFrame();
		}
		catch (const EmulationAbort& abort)
		{
			m_abort = abort;
		}
	}

private:
	void emulateFrame()
	{
		CpuCycle cpuCycle{};
		while (cpuCycle < CpuCyclesPerFrame)
		{
			cpuCycle += Mos6502::In::Step(m_hardware);
		}
	}
};

namespace Nes
{
	HwFrame::HwFrame() :
		p_impl(std::make_shared<Impl>())
	{
	}

	bool HwFrame::StartRomFile(FilePathView romPath)
	{
		return p_impl->StartRomFile(romPath);
	}

	void HwFrame::ControlFrames()
	{
		p_impl->ControlFrames();
	}

	Optional<EmulationAbort> HwFrame::GetAbort() const
	{
		return p_impl->m_abort;
	}

	const Hardware& HwFrame::GetEnv()
	{
		return p_impl->m_hardware;
	}
}
