#pragma once

namespace Gui
{
	namespace GuiScripting_detail
	{
		enum SyntaxType
		{
			Syntax_Plain = 0,
			Syntax_Comment,
			Syntax_Number,
			Syntax_count,
		};

		struct LineCode
		{
			String code{};
			Array<SyntaxType> syntax{};
		};

		int ReplaceTab(LineCode& code, const String& tabSpace = U"    ");
		void ApplySyntax(LineCode& code);
	}
}
