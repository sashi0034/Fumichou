#pragma once

namespace FontKeys
{
	constexpr AssetNameView ZxProto_16_Bitmap = U"ZxProto_16_Bitmap";

	inline void Register()
	{
		FontAsset::Register(ZxProto_16_Bitmap, 16, U"asset/font/0xProto/0xProto-Regular.ttf");
	}
}
