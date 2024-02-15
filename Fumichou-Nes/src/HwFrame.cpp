#include "stdafx.h"
#include "HwFrame.h"

#include "Cartridge.h"
#include "Cartridge_In.h"
#include "HardwareConstants.h"
#include "Mmu_In.h"
#include "Mos6502_In.h"

struct Nes::HwFrame::Impl
{
	inline static Impl* s_instance = nullptr;

	Hardware m_hardware{};
	Optional<EmulationAbort> m_abort{none};
	uint64 m_frameCount{};
	uint64 m_cycleCount{};
	bool m_paused{};

	Impl()
	{
		if (not s_instance) s_instance = this;
	}

	~Impl()
	{
		if (s_instance == this) s_instance = nullptr;
	}

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
		if (m_paused) return;

		// TODO
		try
		{
			emulateFrame();
			m_frameCount++;
		}
		catch (const EmulationAbort& abort)
		{
			m_abort = abort;
		}
	}

private:
	void emulateFrame()
	{
		const auto endCycle = m_cycleCount + CpuCyclesPerFrame_29781;
		while (m_cycleCount < endCycle)
		{
			m_cycleCount += Mos6502::In::Step(m_hardware);
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

	HwFrameView HwFrame::Instance()
	{
		assert(Impl::s_instance);
		return Impl::s_instance;
	}

	HwFrameView::HwFrameView(HwFrame::Impl* impl) :
		p_impl(impl)
	{
	}

	Optional<EmulationAbort> HwFrameView::GetAbort() const
	{
		return p_impl->m_abort;
	}

	const Hardware& HwFrameView::GetHw() const
	{
		return p_impl->m_hardware;
	}

	uint64 HwFrameView::GetFrameCount() const
	{
		return p_impl->m_frameCount;
	}

	uint64 HwFrameView::GetCycleCount() const
	{
		return p_impl->m_cycleCount;
	}

	bool HwFrameView::IsPaused() const
	{
		return p_impl->m_paused;
	}

	void HwFrameView::SetPaused(bool paused)
	{
		p_impl->m_paused = paused;
	}
}
