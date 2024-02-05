# include <Siv3D.hpp> // Siv3D v0.6.12

#include "HwFrame.h"

void Main()
{
	Window::SetTitle(U"Fumichou");
	Scene::SetBackground(ColorF{0.7});

	Nes::HwFrame m_frame{};

	Print(m_frame.GetEnv().GetMemory().GetInternalRam()[0]);

	while (System::Update())
	{
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
