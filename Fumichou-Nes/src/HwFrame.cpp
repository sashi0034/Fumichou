#include "stdafx.h"
#include "HwFrame.h"

#include "Cartridge.h"
#include "Cartridge_In.h"
#include "HardwareConstants.h"
#include "Mmu_In.h"
#include "Mos6502_In.h"
#include "Ppu_In.h"

using namespace Nes;

namespace
{
	constexpr double fps_60 = 60.0;
	constexpr double frameDeltaTime = 1 / fps_60;
}

struct HwFrame::Impl
{
	inline static Impl* s_instance = nullptr;

	Hardware m_hardware{};
	s3d::Optional<EmulationAbort> m_abort{s3d::none};
	uint64 m_frameCount{};
	uint64 m_cycleCount{};
	bool m_paused{};
	double m_controlledTime{};

	Impl()
	{
		if (not s_instance) s_instance = this;
	}

	~Impl()
	{
		if (s_instance == this) s_instance = nullptr;
	}

	bool StartRomFile(s3d::FilePathView romPath)
	{
		if (not Cartridge::In::LoadRomFile(m_hardware.GetCartridge(), romPath))
		{
			return false;
		}

		Ppu::In::UpdateMirroring(m_hardware.GetPpu(), m_hardware.GetCartridge().GetRomData().GetNameTableMirror());

		Mmu::In::MapWholeAddr(m_hardware);

		Mos6502::In::Reset(m_hardware);

		return true;
	}

	void ControlFrames()
	{
		if (m_abort.has_value()) return;
		if (m_paused) return;

		// 60FPS制御
		m_controlledTime += s3d::Scene::DeltaTime();
		while (m_controlledTime >= frameDeltaTime)
		{
			m_controlledTime -= frameDeltaTime;
			if (not StepFrame()) break;
		}
	}

	// 1フレーム実行
	bool StepFrame()
	{
		try
		{
			emulateFrame();
			m_frameCount++;
			return true;
		}
		catch (const EmulationAbort& abort)
		{
			m_abort = abort;
			return false;
		}
	}

	// 1サイクル実行
	void StepCycle()
	{
		try
		{
			stepCycleInternal();
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
			stepCycleInternal();
		}
	}

	void stepCycleInternal()
	{
		// CPU実行
		const CpuCycle cpuCycle = Mos6502::In::Step(m_hardware);

		// PPU実行
		Ppu::In::Step(m_hardware, cpuCycle * 3);

		m_cycleCount += cpuCycle;
	}
};

namespace Nes
{
	HwFrame::HwFrame() :
		p_impl(std::make_shared<Impl>())
	{
	}

	bool HwFrame::StartRomFile(s3d::FilePathView romPath)
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

	s3d::Optional<EmulationAbort> HwFrameView::GetAbort() const
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

	void HwFrameView::StepOneCycle()
	{
		p_impl->StepCycle();
	}

	void HwFrameView::StepOneFrame()
	{
		p_impl->StepFrame();
	}
}
