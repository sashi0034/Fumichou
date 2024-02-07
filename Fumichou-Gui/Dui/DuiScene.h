#pragma once
#include "HwFrame.h"

namespace Dui
{
	class DuiScene
	{
	public:
		DuiScene();
		void Update(Nes::HwFrame& nes);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
