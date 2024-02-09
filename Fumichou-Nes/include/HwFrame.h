#pragma once
#include "Hardware.h"

namespace Nes
{
	class HwFrame
	{
	public:
		HwFrame();

		[[nodiscard]]
		bool StartRomFile(FilePathView romPath);

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
		Optional<EmulationAbort> GetAbort() const;
		const Hardware& GetHw() const;

	private:
		HwFrame::Impl* p_impl;
	};
}
