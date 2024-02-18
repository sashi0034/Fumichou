#include "stdafx.h"
#include "GuiController.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "GuiStatus.h"
#include "GuiMapping.h"
#include "GuiNametable.h"
#include "GuiPatternTable.h"
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
	struct
	{
		WidgetTabBar tab{};
		int tabIndex{};
		GuiTrace trace{};
		GuiNametable nametable{};
	} m_right;

	struct
	{
		WidgetTabBar tab{};
		int tabIndex{};
		GuiStatus generalStatus{};
		GuiMapping mapping{};
	} m_left;

	struct
	{
		GuiPatternTable patternTable{};
	} m_bottom;

	void Update()
	{
		const int screenMargin = getToml<int>(U"screenMargin");
		const int smallMargin = getToml<int>(U"smallMargin");
		const auto sideBg = getToml<ColorF>(U"sideBg");

		constexpr auto screenSize = Nes::Display_256x240 * 3; // + Nes::Display_256x240 / 2;
		const auto [sideWidth, sideHeight] = (Scene::Size() - screenSize) / 2 - Point::One() * screenMargin;

		constexpr int tabHeight = LineHeight * 1;

		// 右領域更新
		{
			const auto available = Point(sideWidth, Scene::Size().y);
			static const std::array tabNames = {U"Trace"_s, U"Nametable"_s};
			const auto rightX = Scene::Size().x - sideWidth;
			(void)Rect(available).moveBy(rightX, 0).rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);
			m_right.tab.Update({
				.availableRect = Rect(available.withY(tabHeight)).moveBy(rightX, 0),
				.currentIndex = m_right.tabIndex,
				.tabNames = tabNames,
			});
			Transformer2D t{Mat3x2::Translate(rightX, tabHeight), TransformCursor::Yes};
			const auto contentAvailable = available.withY(available.y - tabHeight - LineHeight);
			switch (m_right.tabIndex)
			{
			case 0:
				m_right.trace.Update(contentAvailable);
				break;
			case 1:
				m_right.nametable.Update(contentAvailable);
				break;
			default: break;;
			}
		}

		// 左領域更新
		{
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

		{
			// 下側領域
			const auto tl = Point(sideWidth, Scene::Size().y - sideHeight);
			const auto available = Size{Scene::Size().x - sideWidth * 2, sideHeight};
			const auto bg = Rect(available.movedBy(0, smallMargin)).movedBy(tl.x, tl.y - smallMargin);
			(void)bg.rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);
			const ScopedViewport2D viewport2D{tl, available};
			const Transformer2D transformer{Mat3x2::Identity(), Mat3x2::Translate(tl)};
			m_bottom.patternTable.Update(available);
		}

		auto&& nes = Nes::HwFrame::Instance();

		{
			// 画面描画
			const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};
			nes.GetHw().GetPpu().GetVideo().texture.resized(screenSize).drawAt(Scene::Center());
		}

		if (const auto abort = nes.GetAbort())
		{
			// アボートメッセージ
			auto abortText = FontAsset(FontKeys::ZxProto_20_Bitmap)(abort->what());
			(void)abortText.regionAt(Scene::Center()).stretched(4).rounded(2).draw(ColorF(ColorRed, 0.9));
			(void)abortText.drawAt(Scene::Center(), Palette::White);
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
