#pragma once

namespace Nes
{
	namespace ShaderKeys
	{
		constexpr constexpr inline StringView bg_render = U"shader-nes/bg_render";
		constexpr constexpr inline StringView sprite_render = U"shader-nes/sprite_render";

		constexpr inline std::array AllPS = {
			bg_render,
			sprite_render
		};

		constexpr inline std::array AllVS = {
			U""_sv // TODO
		};
	}
}
