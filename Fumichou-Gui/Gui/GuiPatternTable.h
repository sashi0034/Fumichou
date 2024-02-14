#pragma once

namespace Gui
{
	class GuiPatternTable
	{
	public:
		GuiPatternTable();
		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
