#include "stdafx.h"
#include "GuiForward.h"

namespace
{
	bool s_mouseCaptured{};

	class GuiAddon : public IAddon
	{
	public:
		~GuiAddon() override = default;

		bool update() override
		{
			s_mouseCaptured = false;
			return true;
		}
	};
}

void Gui::RegisterGuiAddon()
{
	Addon::Register<GuiAddon>(U"GuiAddon");
}

void Gui::AcceptMouseCaptured()
{
	s_mouseCaptured = true;
}

bool Gui::IsMouseCaptured()
{
	return s_mouseCaptured;
}
