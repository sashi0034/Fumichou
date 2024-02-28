﻿#pragma once

namespace Gui
{
	class GuiController
	{
	public:
		GuiController();

		void Update();

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
