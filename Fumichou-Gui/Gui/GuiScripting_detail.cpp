#include "stdafx.h"
#include "GuiScripting_detail.h"

namespace Gui::GuiScripting_detail
{
	void ApplySyntax(LineCode& code)
	{
		if (code.syntax.size() != code.code.size())
		{
			code.syntax.resize(code.code.size());
		}

		code.syntax.fill(Syntax_Plain);

		// コメント処理
		if (const auto commentStart = code.code.indexOf(U"//"); commentStart != String::npos)
		{
			for (auto i = commentStart; i < code.syntax.size(); ++i) code.syntax[i] = Syntax_Comment;
		}
	}
}
