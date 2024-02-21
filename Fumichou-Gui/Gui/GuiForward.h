#pragma once

namespace Gui
{
	constexpr ColorF ColorGreen{U"#60E010"};
	constexpr ColorF ColorPurple{U"#FF00DC"};
	constexpr ColorF ColorOrange{U"#FF7E01"};
	constexpr ColorF ColorBlue{U"#018AD3"};
	constexpr ColorF ColorRed{U"#EB261F"};

	constexpr int LineHeight = 24;

	inline void EnsureRegisterEmoji(const Emoji& emoji)
	{
		if (not TextureAsset::IsRegistered(emoji.codePoints))
		{
			TextureAsset::Register(emoji.codePoints, emoji, TextureDesc::Mipped);
		}
	}

	void RegisterGuiAddon();

	void AcceptClickCaptured();
	[[nodiscard]] bool IsClickCaptured();
}
