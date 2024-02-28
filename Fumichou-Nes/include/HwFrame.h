#pragma once
#include "Hardware.h"

namespace Nes
{
	class HwFrame
	{
	public:
		HwFrame();

		[[nodiscard]] bool StartRomFile(s3d::FilePathView romPath);

		void ControlFrames();

		friend class HwFrameView;
		static HwFrameView Instance();

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};

	class HwFrameView
	{
	public:
		HwFrameView(HwFrame::Impl* impl);

		void RerenderVideo();

		[[nodiscard]] bool StartRomFile(s3d::FilePathView romPath);
		[[nodiscard]] bool ResetRomFile();

		[[nodiscard]] s3d::Optional<EmulationAbort> GetAbort() const;
		[[nodiscard]] const Hardware& GetHw() const;

		[[nodiscard]] uint64 GetFrameCount() const;
		[[nodiscard]] uint64 GetCycleCount() const;

		[[nodiscard]] bool IsPaused() const;
		void SetPaused(bool paused);

		void StepOneCycle();
		void StepOneFrame();

	private:
		HwFrame::Impl* p_impl;
	};
}
