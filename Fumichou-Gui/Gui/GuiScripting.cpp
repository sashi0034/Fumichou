#include "stdafx.h"
#include "GuiScripting.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "GuiScripting_detail.h"
#include "WidgetSlideBar.h"
#include "Util/KeyRepetition.h"
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

	struct SelectionData
	{
		int startRow;
		int startColumn;
		int endRow;
		int endColumn;
	};

	struct EditState
	{
		int row{};
		int column{};

		struct
		{
			int row{};
			int column{};
		} select;

		bool isSelecting{};

		SelectionData GetSelection() const
		{
			if (row < select.row || (row == select.row && column < select.column))
			{
				return SelectionData{
					.startRow = row,
					.startColumn = column,
					.endRow = select.row,
					.endColumn = select.column
				};
			}
			else
			{
				return SelectionData{
					.startRow = select.row,
					.startColumn = select.column,
					.endRow = row,
					.endColumn = column
				};
			}
		}
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
	bool m_isEditing{};
	EditState m_edit{};
	double m_cursorFlash{};

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

		if (m_isEditing && m_edit.isSelecting)
		{
			// 選択領域描画
			int lineIndex = m_headIndex - 1;
			const auto selection = m_edit.GetSelection();

			Console.writeln(U"{} {} {} {}"_fmt(selection.startRow, selection.startColumn, selection.endRow,
			                                   selection.endColumn));

			for (int y = 0; y < availableRegion.y; y += LineHeight)
			{
				lineIndex++;
				double leftX = codeLeft;
				double rightX = availableRegion.x;
				if (lineIndex < selection.startRow || selection.endRow < lineIndex)
				{
					// 範囲外
					continue;
				}
				if (lineIndex == selection.endRow)
				{
					// 選択終了点
					rightX = leftX;
					auto&& lineGlyphs = font.getGlyphs(m_lines[lineIndex].code);
					for (int x = 0; x < selection.endColumn; ++x) rightX += lineGlyphs[x].xAdvance;
				}
				if (lineIndex == selection.startRow)
				{
					// 選択開始点
					auto&& lineGlyphs = font.getGlyphs(m_lines[lineIndex].code);
					for (int x = 0; x < selection.startColumn; ++x) leftX += lineGlyphs[x].xAdvance;
				}

				(void)Rect(leftX, y, rightX - leftX, LineHeight).draw(Palette::Steelblue);
			}
		}

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
		if (m_isEditing)
		{
			updateEdit(indexTail);
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
				m_isEditing = false;
				m_edit.isSelecting = false;
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

	void updateEdit(int indexTail)
	{
		const int prevR = m_edit.row;
		const int prevC = m_edit.column;

		// カーソル移動
		if (moveEditCursor(indexTail))
		{
			if (not m_edit.isSelecting && KeyShift.pressed())
			{
				// 選択開始
				m_edit.isSelecting = true;
				m_edit.select.row = prevR;
				m_edit.select.column = prevC;
			}
			else if (m_edit.isSelecting && not KeyShift.pressed())
			{
				m_edit.isSelecting = false;
			}
			clampCursor(indexTail);
			m_cursorFlash = 0;
		}

		const auto font = getFont();
		const int codeLeft = getCodeLeft();
		const int newC = TextInput::UpdateText(m_lines[m_edit.row].code, m_edit.column);
		if (newC != m_edit.column)
		{
			// 変更が存在
			m_edit.column = newC;

			// タブ処理
			const int replacedTabs = ReplaceTab(m_lines[m_edit.row]);
			if (replacedTabs != 0) m_edit.column += replacedTabs - 1;

			// 改行処理
			processEditWrap();

			ApplySyntax(m_lines[m_edit.row]);
			clampCursor(indexTail);
		}
		else if (m_edit.row > 0 && m_edit.column == 0 && Util::IsKeyRepeating(KeyBackspace))
		{
			// 行削除
			m_edit.column = m_lines[m_edit.row - 1].code.size();
			m_lines[m_edit.row - 1].code += m_lines[m_edit.row].code;
			m_lines.remove_at(m_edit.row);
			m_edit.row--;
			ApplySyntax(m_lines[m_edit.row]);
			clampCursor(indexTail);
		}

		m_cursorFlash += Scene::DeltaTime();
		if (Periodic::Square0_1(0.5s, m_cursorFlash) != 0)
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

	bool moveEditCursor(int indexTail)
	{
		if (KeyHome.down())
		{
			m_edit.column = 0;
		}
		else if (KeyEnd.down())
		{
			m_edit.column = INT_MAX;
		}
		else if (KeyPageUp.down())
		{
			m_edit.column -= indexTail;
		}
		else if (KeyPageDown.down())
		{
			m_edit.column += indexTail;
		}
		else if (Util::IsKeyRepeating(KeyUp))
		{
			m_edit.row -= KeyUp.pressedDuration().count() > 1.0 ? 2 : 1;
		}
		else if (Util::IsKeyRepeating(KeyDown))
		{
			m_edit.row += KeyDown.pressedDuration().count() > 1.0 ? 2 : 1;
		}
		else if (Util::IsKeyRepeating(KeyLeft))
		{
			m_edit.column -= KeyLeft.pressedDuration().count() > 1.0 ? 2 : 1;
			if (m_edit.column < 0 && m_edit.row > 0)
			{
				m_edit.column = INT_MAX;
				m_edit.row--;
			}
		}
		else if (Util::IsKeyRepeating(KeyRight))
		{
			m_edit.column += KeyRight.pressedDuration().count() > 1.0 ? 2 : 1;
			if (m_edit.column >= m_lines[m_edit.row].code.size() && m_edit.column < m_lines.size() - 1)
			{
				m_edit.column = 0;
				m_edit.row++;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	void clampCursor(int indexTail)
	{
		m_edit.row = std::min<int>(std::max(m_edit.row, 0), m_lines.size() - 1);
		m_edit.column = std::min<int>(std::max(m_edit.column, 0), m_lines[m_edit.row].code.size());

		if (m_edit.row - m_headIndex < 0)
		{
			m_headIndex = m_edit.row;
		}
		if (m_edit.row - m_headIndex >= indexTail - 1)
		{
			m_headIndex = m_edit.row - indexTail + 1;
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
				m_isEditing = true;
				m_edit.row = y + m_headIndex;
				if (m_edit.row >= m_lines.size())
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

	void processEditWrap()
	{
		const auto indentSpace = m_lines[m_edit.row].code.substr(0, m_lines[m_edit.row].code.indexOfNot(U' '));

		while (true)
		{
			// 改行処理
			const auto wrapAt = m_lines[m_edit.row].code.indexOfAny(U"\r\n");
			if (wrapAt == String::npos) break;
			m_lines[m_edit.row].code.remove_at(wrapAt);

			auto newLine = LineCode{.code = indentSpace + m_lines[m_edit.row].code.substr(wrapAt)};
			m_lines[m_edit.row].code = m_lines[m_edit.row].code.substr(0, wrapAt);

			ApplySyntax(m_lines[m_edit.row]);

			// 次行の処理へ
			m_edit.column = indentSpace.size();
			m_edit.row++;
			m_lines.insert(m_lines.begin() + m_edit.row, newLine);
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
