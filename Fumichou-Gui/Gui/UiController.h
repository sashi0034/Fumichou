#pragma once

namespace Gui
{
	class UiController
	{
	public:
		UiController();

		void Update();

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
