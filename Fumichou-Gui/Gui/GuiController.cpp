#include "stdafx.h"
#include "GuiController.h"

#include "FontKeys.h"

using namespace Gui;

struct GuiController::Impl
{
};

namespace Gui
{
	GuiController::GuiController() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiController::Update()
	{
		auto&& nes = Nes::HwFrame::Instance();
		if (const auto abort = nes.GetAbort())
		{
			(void)FontAsset(FontKeys::ZxProto_16_Bitmap)(abort->what()).drawAt(Scene::Center());
		}
	}
}
