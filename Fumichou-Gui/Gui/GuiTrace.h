#pragma once

namespace Gui
{
	class GuiTrace
	{
	public:
		GuiTrace();

		void Update(const SizeF& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
