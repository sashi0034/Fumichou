#pragma once

namespace FontKeys
{
	constexpr AssetNameView ZxProto_20_Bitmap = U"ZxProto_20_Bitmap";
	constexpr AssetNameView Icon_40_Msdf = U"Icon_40_Msdf";

	inline void Register()
	{
		FontAsset::Register(ZxProto_20_Bitmap, 20, U"asset/font/0xProto/0xProto-Regular.ttf");
		FontAsset::Register(Icon_40_Msdf, FontMethod::MSDF, 40, Typeface::Icon_MaterialDesign);
	}
}
