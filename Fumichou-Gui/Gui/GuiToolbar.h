#pragma once

namespace Gui
{
	class GuiToolbar
	{
	public:
		GuiToolbar();
		void Update(const Size& availableRegion);

		bool ShowSpriteWireframe() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
