#include "stdafx.h"
#include "GuiController.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "GuiStatus.h"
#include "GuiMapping.h"
#include "GuiTrace.h"
#include "WidgetTabBar.h"
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

	struct
	{
		WidgetTabBar tab{};
		int tabIndex{};
		GuiStatus generalStatus{};
		GuiMapping mapping{};
	} m_left;

	void Update()
	{
		const int sideWidth = getToml<int>(U"sideWidth");
		const auto sideBg = getToml<ColorF>(U"sideBg");

		// 右領域更新
		{
			Transformer2D t{Mat3x2::Translate(Scene::Size().x - sideWidth, 0), TransformCursor::Yes};
			const auto available = Scene::Size().withX(sideWidth);
			(void)Rect(available).rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);
			m_trace.Update(available);
		}

		// 左領域更新
		{
			constexpr int tabHeight = LineHeight * 1;
			auto available = Size{sideWidth, Scene::Size().y};

			(void)Rect(available).rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);

			static const std::array tabNames = {U"Status"_s, U"Mapping"_s};
			m_left.tab.Update({
				.availableRect = Rect(available.withY(tabHeight)),
				.currentIndex = m_left.tabIndex,
				.tabNames = tabNames,
			});

			available.y -= tabHeight;
			Transformer2D t{Mat3x2::Translate(0, tabHeight), TransformCursor::Yes};
			switch (m_left.tabIndex)
			{
			case 0:
				m_left.generalStatus.Update(available);
				break;
			case 1:
				m_left.mapping.Update(available);
				break;
			default: break;
			}
		}

		// アボートメッセージ
		auto&& nes = Nes::HwFrame::Instance();
		if (const auto abort = nes.GetAbort())
		{
			(void)FontAsset(FontKeys::ZxProto_20_Bitmap)(abort->what()).drawAt(Scene::Center(), ColorRed);
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
		p_impl->Update();
	}
}
