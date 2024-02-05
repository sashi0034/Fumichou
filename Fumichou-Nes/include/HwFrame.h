#pragma once
#include "Hardware.h"

namespace Nes
{
	class HwFrame
	{
	public:
		HwFrame();

		void LoadRomFile(FilePathView romPath);

		const Hardware& GetEnv();

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
