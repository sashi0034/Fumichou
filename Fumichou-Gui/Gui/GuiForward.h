#pragma once

namespace Gui
{
	inline constexpr Size SceneSize_1920x1080{1920, 1080};

	inline constexpr ColorF ColorGreen{U"#60E010"};
	inline constexpr ColorF ColorPurple{U"#FF00DC"};
	inline constexpr ColorF ColorOrange{U"#FF7E01"};
	inline constexpr ColorF ColorBlue{U"#018AD3"};
	inline constexpr ColorF ColorRed{U"#EB261F"};

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
