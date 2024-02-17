#pragma once

namespace Nes
{
	namespace ShaderKeys
	{
		constexpr constexpr inline StringView bg_render = U"shader-nes/bg_render";
		constexpr constexpr inline StringView bg_rendering = U"shader-nes/bg_rendering";

		constexpr inline std::array AllPS = {
			bg_render,
			bg_rendering
		};

		constexpr inline std::array AllVS = {
			bg_rendering
		};
	}
}
