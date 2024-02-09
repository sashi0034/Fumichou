#include "stdafx.h"
#include "GuiController.h"

#include "FontKeys.h"
#include "GuiTrace.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"Controller." + key);
	}
}

struct GuiController::Impl
{
	GuiTrace m_trace{};

	void Updaate()
	{
		const int sideWidth = getToml<int>(U"sideWidth");

		{
			Transformer2D t{Mat3x2::Translate(Scene::Size().x - sideWidth, 0)};
			m_trace.Update(Scene::Size().withX(sideWidth));
		}

		// アボートメッセージ
		auto&& nes = Nes::HwFrame::Instance();
		if (const auto abort = nes.GetAbort())
		{
			(void)FontAsset(FontKeys::ZxProto_20_Bitmap)(abort->what()).drawAt(Scene::Center());
		}
	}
};

namespace Gui
{
	GuiController::GuiController() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiController::Update()
	{
		p_impl->Updaate();
	}
}
