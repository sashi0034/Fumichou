#pragma once

namespace Gui
{
	class GuiSetting
	{
	public:
		GuiSetting();
		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
