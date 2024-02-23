﻿#pragma once

namespace Gui
{
	namespace GuiScripting_detail
	{
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

		void ApplySyntax(LineCode& code);
	}
}
