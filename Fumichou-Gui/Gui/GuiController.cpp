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
		return Util::GetTomlStyle<T>(U"GuiController." + key);
	}
}

struct GuiController::Impl
{
	GuiTrace m_trace{};

	void Updaate()
	{
		const int sideWidth = getToml<int>(U"sideWidth");
		const auto sideBg = getToml<ColorF>(U"sideBg");

		{
			Transformer2D t{Mat3x2::Translate(Scene::Size().x - sideWidth, 0), TransformCursor::Yes};
			const auto available = Scene::Size().withX(sideWidth);
			(void)Rect(available).rounded(4).draw(sideBg).drawFrame(1, sideBg * 1.1f);
			m_trace.Update(available);
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
