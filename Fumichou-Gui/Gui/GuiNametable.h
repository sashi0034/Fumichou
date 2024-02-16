#pragma once

namespace Gui
{
	class GuiNametable
	{
	public:
		GuiNametable();

		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
