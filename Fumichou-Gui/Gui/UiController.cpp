#include "stdafx.h"
#include "UiController.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "GuiStatus.h"
#include "GuiMapping.h"
#include "GuiNametable.h"
#include "GuiPatternTable.h"
#include "GuiToolbar.h"
#include "GuiTrace.h"
#include "HwFrame.h"
#include "JoyBackground.h"
#include "UiScreenOverlay.h"
#include "WidgetTabBar.h"
#include "Details/RenderSpriteWireframe.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"UiController." + key);
	}

	constexpr int adjustScale_3 = 3;
}

struct UiController::Impl
{
	int m_modeIndex{};
	UiScreenOverlay m_screenOverlay{};
	double m_screenScaling{};

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
		// GuiScripting scripting{};
	} m_left;

	struct
	{
		GuiPatternTable patternTable{};
	} m_bottom;

	struct
	{
		GuiToolbar toolbar{};
	} m_top;

	struct
	{
		JoyBackground bg{};
	} m_joy;

	void Update()
	{
		// 周囲描画
		if (m_modeIndex == 0) updateGui(Nes::Display_256x240 * adjustScale_3);
		else updateJoy(Nes::Display_256x240 * adjustScale_3);

		// スクローンサイズ調整
		constexpr double scalingSpeed = 3.0;
		if (m_modeIndex == 0) m_screenScaling -= scalingSpeed * Scene::DeltaTime();
		else m_screenScaling += scalingSpeed * Scene::DeltaTime();
		m_screenScaling = Math::Clamp(m_screenScaling, 0, 1);
		const auto screenSize =
			(Nes::Display_256x240 * (adjustScale_3 + 1.0 * EaseInOutBack(m_screenScaling))).asPoint();

		if (0 < m_screenScaling && m_screenScaling < 1)
		{
			// モード移行アニメーション中は背景をぼやかす
			Rect(Scene::Size()).draw(ColorF(0.3, 0.3));
		}

		// ゲーム画面描画
		updateScreen(screenSize);
	}

private:
	void updateScreen(const Point screenSize)
	{
		auto&& nes = Nes::HwFrame::Instance();

		{
			// 画面描画
			const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};
			const auto rect = nes.GetHw().GetPpu().GetVideo().resized(screenSize).drawAt(Scene::Center());
			if (m_modeIndex == 0) (void)rect.stretched(2).drawFrame(2, Palette::Black);

			m_screenOverlay.Update({
				.screenRect = rect,
				.clickedModeChange = [this]() { m_modeIndex = (m_modeIndex + 1) % 2; },
			});

			if (m_top.toolbar.ShowSpriteWireframe()) Details::RenderSpriteWireframe(rect);
		}

		if (const auto abort = nes.GetAbort())
		{
			// アボートメッセージ
			const auto abortText = FontAsset(FontKeys::ZxProto_20_Bitmap)(abort->what());
			(void)abortText.regionAt(Scene::Center()).stretched(4).rounded(2).draw(ColorF(ColorRed, 0.9));
			(void)abortText.drawAt(Scene::Center(), Palette::White);
		}
	}

	void updateGui(const Size& screenSize)
	{
		const int screenMargin = getToml<int>(U"screenMargin");
		// const int smallMargin = getToml<int>(U"smallMargin");
		const auto sideBg = getToml<ColorF>(U"sideBg");

		const auto [sideWidth, sideHeight] = (Scene::Size() - screenSize) / 2 - Point::One() * screenMargin;

		constexpr int tabHeight = LineHeight * 1;

		// 背景黒
		(void)Rect(Scene::Size()).draw(sideBg);

		// 右領域更新
		{
			const auto available = Point(sideWidth, Scene::Size().y);
			static const std::array tabNames = {U"Trace"_s, U"Nametable"_s};
			const auto rightX = Scene::Size().x - sideWidth;
			// (void)Rect(available).moveBy(rightX, 0).rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);
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
			// (void)Rect(available).rounded(4).draw(sideBg).stretched(1).drawFrame(2, sideBg * 1.1f);
			static const std::array tabNames = {U"Status"_s, U"Mapping"_s};
			m_left.tab.Update({
				.availableRect = Rect(available.withY(tabHeight)),
				.currentIndex = m_left.tabIndex,
				.tabNames = tabNames,
			});

			available.y -= tabHeight;
			constexpr auto tl = Point(0, tabHeight);
			const ScopedViewport2D viewport2D{tl, available};
			const Transformer2D t{Mat3x2::Identity(), Mat3x2::Translate(tl)};
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
			// 上側領域
			const auto tl = Point(sideWidth, 0);
			const auto available = Size{Scene::Size().x - sideWidth * 2, sideHeight};
			// const ScopedViewport2D viewport2D{tl, available};
			// const Transformer2D transformer{Mat3x2::Identity(), Mat3x2::Translate(tl)};
			const Transformer2D transformer{Mat3x2::Translate(tl), TransformCursor::Yes};
			m_top.toolbar.Update(available);
		}

		{
			// 下側領域
			const auto tl = Point(sideWidth, Scene::Size().y - sideHeight);
			const auto available = Size{Scene::Size().x - sideWidth * 2, sideHeight};
			const ScopedViewport2D viewport2D{tl, available};
			const Transformer2D transformer{Mat3x2::Identity(), Mat3x2::Translate(tl)};
			m_bottom.patternTable.Update(available);
		}
	}

	void updateJoy(const Size& screenSize)
	{
		m_joy.bg.Update(Scene::Size());
	}
};

namespace Gui
{
	UiController::UiController() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void UiController::Update()
	{
		p_impl->Update();
	}
}
