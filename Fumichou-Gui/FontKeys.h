#pragma once

namespace FontKeys
{
	constexpr AssetNameView ZxProto_20_Bitmap = U"ZxProto_20_Bitmap";

	inline void Register()
	{
		FontAsset::Register(ZxProto_20_Bitmap, 20, U"asset/font/0xProto/0xProto-Regular.ttf");
	}
}
