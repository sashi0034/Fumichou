#include "stdafx.h"
#include "GuiForward.h"

namespace
{
	bool s_clickCaptured{};

	class GuiAddon : public IAddon
	{
	public:
		~GuiAddon() override = default;

		bool update() override
		{
			s_clickCaptured = false;
			return true;
		}
	};
}

void Gui::RegisterGuiAddon()
{
	Addon::Register<GuiAddon>(U"GuiAddon");
}

void Gui::AcceptClickCaptured()
{
	s_clickCaptured = true;
}

bool Gui::IsClickCaptured()
{
	return s_clickCaptured;
}
