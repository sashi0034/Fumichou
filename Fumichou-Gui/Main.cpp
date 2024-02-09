﻿#include "stdafx.h"

#define CATCH_CONFIG_RUNNER
#include <ThirdParty/Catch2/catch.hpp>

#include "FontKeys.h"
#include "HwFrame.h"
#include "Utils\Utils.h"

namespace
{
	void setupWindow()
	{
		Window::SetTitle(U"Fumichou");
		Window::SetStyle(WindowStyle::Sizable);
		Scene::Resize(1920, 1080);
		Window::Resize(1280, 720);
		Scene::SetResizeMode(ResizeMode::Keep);
		Scene::SetBackground(ColorF{0.3});
		System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	}
}

void Main()
{
	const bool isPassedTests = Catch::Session().run() == 0;
	if (!isPassedTests)
	{
		Utils::WaitAnyKeyOnConsole();
	}

	setupWindow();

	FontKeys::Register();

	auto args = System::GetCommandLineArgs();

	FilePath romPath = args.size() >= 2 ? args[1] : U"";
	if (not FileSystem::Exists(romPath))
	{
		romPath = Dialog::OpenFile({}, FileSystem::CurrentDirectory() + U"rom").
			value_or(U"");
	}

	Console.open();
	Nes::HwFrame nes{};
	if (nes.StartRomFile(romPath) == false)
	{
		Utils::WaitAnyKeyOnConsole();
	}

	Console.writeln(U"Process started.");

	while (System::Update())
	{
		nes.ControlFrames();
		if (const auto abort = nes.GetAbort())
		{
			(void)FontAsset(FontKeys::ZxProto_16_Bitmap)(abort->what()).drawAt(Scene::Center());
		}
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
