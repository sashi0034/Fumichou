#pragma once

namespace Gui
{
	constexpr ColorF ColorGreen{U"#60E010"};
	constexpr ColorF ColorPurple{U"#FF00DC"};
	constexpr ColorF ColorOrange{U"#FF7E01"};
	constexpr ColorF ColorBlue{U"#018AD3"};
	constexpr ColorF ColorRed{U"#EB261F"};

	constexpr int LineHeight = 24;

	void RegisterGuiAddon();

	void AcceptMouseCaptured();
	[[nodiscard]] bool IsMouseCaptured();
}
