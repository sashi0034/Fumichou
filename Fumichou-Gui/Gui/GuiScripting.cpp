﻿#include "stdafx.h"
#include "GuiScripting.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "GuiScripting_detail.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;
using namespace GuiScripting_detail;

namespace
{
	FilePathView scriptFileName{U"scripting.as"};

	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiScripting." + key);
	}

	void drawCodeLine(const Font& font, const LineCode& line, Vec2 penPos)
	{
		static constexpr std::array<ColorF, Syntax_count> colors = {
			Palette::Gainsboro,
			ColorGreen.lerp(ColorF(Palette::Dimgray), 0.1),
			ColorOrange,
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

	struct EditState
	{
		int row{};
		int column{};
	};

	int getCodeLeft()
	{
		return getToml<int>(U"codeLeft");
	}

	FontAsset getFont()
	{
		return FontAsset(FontKeys::ZxProto_20_Bitmap);
	}
}

struct GuiScripting::Impl
{
	Array<LineCode> m_lines{};
	bool m_initialized{};
	WidgetSlideBar m_verticalSlider{};
	int m_headIndex{};
	bool m_editing{};
	EditState m_edit{};

	void Update(const Size& availableRegion)
	{
		if (not m_initialized)
		{
			// 初期化
			m_initialized = true;
			loadScript();
		}

		auto&& font = getFont();
		const int codeLeft = getCodeLeft();

		// 表示領域のライン描画
		int indexTail = 0;
		for (int y = 0; y < availableRegion.y; y += LineHeight)
		{
			const int lineIndex = m_headIndex + indexTail;
			font(Format(lineIndex)).draw(Arg::topLeft = Vec2{8, y}, Palette::Gray);
			if (lineIndex < m_lines.size()) drawCodeLine(font, m_lines[lineIndex], Vec2{codeLeft, y});
			indexTail++;
		}

		// 編集処理
		if (m_editing)
		{
			updateEdit();
		}

		// インデックス移動
		bool hovering{};
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			hovering = true;
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

		// 編集カーソル移動処理
		if (MouseL.down())
		{
			if (not hovering)
			{
				m_editing = false;
			}
			else if (not IsClickCaptured() && MouseL.down())
			{
				moveCursorByClick(availableRegion, indexTail);
			}
		}
	}

private:
	// ファイル読み込み
	void loadScript()
	{
		TextReader reader{scriptFileName};
		m_lines.clear();
		for (auto&& line : reader.readLines())
		{
			auto element = LineCode{.code = line, .syntax = Array<SyntaxType>(line.size())};
			ApplySyntax(element);
			m_lines.emplace_back(element);
		}
	}

	void updateEdit()
	{
		const auto font = getFont();
		const int codeLeft = getCodeLeft();
		const int newC = TextInput::UpdateText(m_lines[m_edit.row].code, m_edit.column);
		if (newC != m_edit.column)
		{
			// 変更
			m_edit.column = newC;
			ApplySyntax(m_lines[m_edit.row]);
		}

		if (Periodic::Square0_1(1.0s) != 0)
		{
			// カーソル描画
			const double cursorThickness = getToml<int>(U"cursorThickness");
			const int y = m_edit.row - m_headIndex;
			auto&& lineGlyphs = font.getGlyphs(m_lines[m_edit.row].code);
			double drawX = codeLeft;
			for (int x = 0; x < m_edit.column; ++x) drawX += lineGlyphs[x].xAdvance;
			(void)Line(drawX, y * LineHeight, drawX, (y + 1) * LineHeight).draw(cursorThickness, Palette::White);
		}
	}

	void moveCursorByClick(const Size& availableRegion, int indexTail)
	{
		const int codeLeft = getCodeLeft();
		const auto font = getFont();
		const auto cursorPos = Cursor::Pos();
		for (int y = 0; y < indexTail; y++)
		{
			const auto collider =
				RectF(Arg::topLeft = Vec2{0, y * LineHeight}, availableRegion.withY(LineHeight));
			if (collider.intersects(cursorPos))
			{
				m_editing = true;
				m_edit.row = y + m_headIndex;
				if (m_edit.row > m_lines.size())
				{
					// 列を末尾にする
					m_edit.row = m_lines.size() - 1;
					m_edit.column = m_lines[m_edit.row].code.size();
				}
				else
				{
					// 列をカーソルXから決定
					auto&& lineGlyphs = font.getGlyphs(m_lines[m_edit.row].code);
					double left = codeLeft;
					m_edit.column = lineGlyphs.size();
					for (int c = 0; c < lineGlyphs.size(); ++c)
					{
						auto&& g = lineGlyphs[c];
						left += g.xAdvance;
						if (cursorPos.x < left)
						{
							m_edit.column = c;
							break;
						}
					}
				}
				break;
			}
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
