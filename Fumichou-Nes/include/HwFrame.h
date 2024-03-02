#pragma once
#include "Hardware.h"

namespace Nes
{
	class HwFrame
	{
	public:
		HwFrame();

		bool StartRomFile(s3d::FilePathView romPath);

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

		bool StartRomFile(s3d::FilePathView romPath);
		[[nodiscard]] bool ResetRomFile();
		[[nodiscard]] s3d::FilePathView CurrentRomFile() const;

		[[nodiscard]] s3d::Optional<EmulationAbort> GetAbort() const;
		void ClearAbort();
		[[nodiscard]] const Hardware& GetHw() const;

		[[nodiscard]] uint64 GetFrameCount() const;
		[[nodiscard]] uint64 GetCycleCount() const;
		[[nodiscard]] double GetRunningTime() const;

		[[nodiscard]] bool IsPaused() const;
		void SetPaused(bool paused);

		void RequestBreakpoint();

		void StepOneCycle();
		void StepOneFrame();

	private:
		HwFrame::Impl* p_impl;
	};
}
