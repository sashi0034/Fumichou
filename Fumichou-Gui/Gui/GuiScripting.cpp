#include "stdafx.h"
#include "GuiScripting.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	FilePathView scriptFileName{U"scripting.as"};

	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiScripting." + key);
	}

	enum SyntaxType
	{
		Syntax_Plain = 0,
		Syntax_Comment,
		Syntax_count,
	};

	struct LineCode
	{
		String code{};
		Array<SyntaxType> syntax{};
	};

	void drawCodeLine(const Font& font, const LineCode& line, Vec2 penPos)
	{
		static constexpr std::array<ColorF, Syntax_count> colors = {
			Palette::Gainsboro,
			ColorGreen.lerp(ColorF(Palette::Dimgray), 0.1),
		};

		// penPos.y += font.height();

		const auto glyphs = font.getGlyphs(line.code);
		for (int i = 0; i < std::min(line.syntax.size(), glyphs.size()); ++i)
		{
			auto&& g = glyphs[i];
			g.texture.draw(Math::Round(penPos + g.getOffset()), colors[line.syntax[i]]);
			penPos.x += g.xAdvance;
		}
	}
}

struct GuiScripting::Impl
{
	Array<LineCode> m_lines{};
	bool m_initialized{};
	WidgetSlideBar m_verticalSlider{};
	int m_headIndex{};

	void Update(const Size& availableRegion)
	{
		if (not m_initialized)
		{
			// 初期化
			m_initialized = true;
			loadScript();
		}

		auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);
		const int codeRight = getToml<int>(U"codeRight");

		// 表示領域のライン描画
		int indexTail = 0;
		for (int y = 0; y < availableRegion.y; y += LineHeight)
		{
			const int lineIndex = m_headIndex + indexTail;
			font(Format(lineIndex)).draw(Arg::topLeft = Vec2{8, y}, Palette::Gray);
			if (lineIndex < m_lines.size()) drawCodeLine(font, m_lines[lineIndex], Vec2{codeRight, y});
			indexTail++;
		}

		// インデックス移動
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			constexpr int step = 1;
			const auto wheel = Mouse::Wheel();
			if (wheel < 0) m_headIndex -= step;
			else if (wheel > 0) m_headIndex += step;
		}

		// 垂直バー
		m_verticalSlider.UpdateVerticalInverted({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_headIndex,
			.minIndex = 0,
			.maxIndex = static_cast<int>(m_lines.size()),
			.pageSize = indexTail
		});
	}

private:
	// ファイル読み込み
	void loadScript()
	{
		TextReader reader{scriptFileName};
		m_lines.clear();
		for (auto&& line : reader.readLines())
		{
			m_lines.emplace_back(LineCode{.code = line, .syntax = Array<SyntaxType>(line.size())});
		}
	}
};

namespace Gui
{
	GuiScripting::GuiScripting() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiScripting::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
