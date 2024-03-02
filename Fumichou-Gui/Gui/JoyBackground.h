#pragma once

namespace Gui
{
	class JoyBackground
	{
	public:
		JoyBackground();
		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
