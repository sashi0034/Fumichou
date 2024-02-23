#include "stdafx.h"
#include "GuiScripting_detail.h"

namespace
{
	using namespace Gui::GuiScripting_detail;

	void parseNumber(LineCode& code, size_t tailIndex)
	{
		bool isHex{};
		for (size_t i = 0; i < tailIndex; ++i)
		{
			const char32_t nextChar = code.code[i];
			if ('0' <= nextChar && nextChar <= '9')
			{
				code.syntax[i] = Syntax_Number;

				// 16進数処理
				if (nextChar == U'0' && i < tailIndex - 1 && code.code[i + 1] == U'x')
				{
					i++;
					code.syntax[i] = Syntax_Number;
					isHex = true;
				}
			}
			else if (isHex && 'A' <= nextChar && nextChar <= 'F')
			{
				code.syntax[i] = Syntax_Number;
			}
			else
			{
				isHex = false;
			}
		}
	}
}

namespace Gui::GuiScripting_detail
{
	void ApplySyntax(LineCode& code)
	{
		if (code.syntax.size() != code.code.size())
		{
			code.syntax.resize(code.code.size());
		}

		code.syntax.fill(Syntax_Plain);

		size_t tailIndex = code.syntax.size();

		// コメント処理
		if (const auto commentStart = code.code.indexOf(U"//"); commentStart != String::npos)
		{
			for (auto i = commentStart; i < code.syntax.size(); ++i) code.syntax[i] = Syntax_Comment;
			tailIndex = commentStart;
		}

		// 数値処理
		parseNumber(code, tailIndex);
	}
}
