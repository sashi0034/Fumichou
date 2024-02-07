#include "stdafx.h"

#define CATCH_CONFIG_RUNNER
#include <ThirdParty/Catch2/catch.hpp>

#include "HwFrame.h"
#include "Dui/DuiScene.h"
#include "Utils\Utils.h"

namespace
{
	void setupWindow()
	{
		Window::SetTitle(U"Fumichou");
		Window::SetStyle(WindowStyle::Sizable);
		Window::Resize(1280, 720);
		Scene::SetResizeMode(ResizeMode::Actual);
		Scene::SetBackground(ColorF{0.3});
		System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	}
}

void Main()
{
	Addon::Register<ImGuiSivAddon>(U"ImGui");

	const bool isPassedTests = Catch::Session().run() == 0;
	if (!isPassedTests)
	{
		Utils::WaitAnyKeyOnConsole();
	}

	setupWindow();

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

	Dui::DuiScene dui{};

	while (System::Update())
	{
		dui.Update(frame);
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
