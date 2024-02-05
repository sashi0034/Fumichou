# include <Siv3D.hpp> // Siv3D v0.6.12

#include "HwFrame.h"

void Main()
{
	Window::SetTitle(U"Fumichou");
	Scene::SetBackground(ColorF{0.7});

	auto args = System::GetCommandLineArgs();

	FilePath romPath = args.size() >= 2 ? args[1] : U"";
	if (not FileSystem::Exists(romPath))
	{
		romPath = Dialog::OpenFile({}, FileSystem::CurrentDirectory() + U"rom").
			value_or(U"");
	}

	Console.open();
	Nes::HwFrame frame{};
	frame.LoadRomFile(romPath);

	Console.writeln(U"Process started.");

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
