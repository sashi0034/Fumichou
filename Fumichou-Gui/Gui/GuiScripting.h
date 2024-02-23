#pragma once

namespace Gui
{
	class GuiScripting
	{
	public:
		GuiScripting();

		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
