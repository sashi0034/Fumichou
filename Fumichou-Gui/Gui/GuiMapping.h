#pragma once

namespace Gui
{
	class GuiMapping
	{
	public:
		GuiMapping();
		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
