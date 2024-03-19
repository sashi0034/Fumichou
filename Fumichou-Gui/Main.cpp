#include "stdafx.h"

#if	 SIV3D_PLATFORM(WINDOWS)
#include <dwmapi.h>
#endif

#define CATCH_CONFIG_RUNNER
#include <ThirdParty/Catch2/catch.hpp>

#include "FontKeys.h"
#include "HwFrame.h"
#include "ShaderKeys.h"
#include "Gui/UiController.h"
#include "Gui/GuiForward.h"
#include "Gui/GuiIni.h"
#include "Util/KeyRepetition.h"
#include "Util/ShaderReloader.h"
#include "Util/TomlStyleSheet.h"
#include "Util\Utils.h"

namespace
{
	void setupWindow()
	{
#if	 SIV3D_PLATFORM(WINDOWS)
		const auto hWnd = static_cast<HWND>(Platform::Windows::Window::GetHWND());
		constexpr DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
		constexpr BOOL DARK_MODE = true;
		DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &DARK_MODE, sizeof(DARK_MODE));
#endif
		Window::SetTitle(U"Fumichou");
		Window::SetStyle(WindowStyle::Sizable);
		Scene::SetResizeMode(ResizeMode::Keep);
		Scene::SetBackground(ColorF{0.3});
		System::SetTerminationTriggers(UserAction::CloseButtonClicked);
		Scene::Resize(Gui::SceneSize_1920x1080);
		Window::Resize(1280, 720);
	}
}

void Main()
{
	const bool isPassedTests = Catch::Session().run() == 0;
	if (!isPassedTests)
	{
		Util::WaitAnyKeyOnConsole();
	}

	setupWindow();
	Gui::LoadGuiIni();

	Util::RegisterTomlStyleSheetAddon();
	Util::RegisterKeyRepetitionAddon();
	Gui::RegisterGuiAddon();

	FontKeys::Register();

	const Font emojiFont{48, Typeface::ColorEmoji};
	(void)FontAsset(FontKeys::ZxProto_20_Bitmap).addFallback(emojiFont);

	for (auto& key : Nes::ShaderKeys::AllPS) PixelShaderAsset::Register(key, HLSL(U"asset/{}.hlsl"_fmt(key)));
	for (auto& key : Nes::ShaderKeys::AllVS) VertexShaderAsset::Register(key, HLSL(U"asset/{}.hlsl"_fmt(key)));

#ifdef _DEBUG
	Util::RegisterShaderReloaderAddon();
#endif

	const auto args = System::GetCommandLineArgs();

	const FilePath romPath = args.size() >= 2 ? args[1] : U"";
	// if (not FileSystem::Exists(romPath))
	// {
	// 	romPath = Dialog::OpenFile({}, FileSystem::CurrentDirectory() + U"rom").
	// 		value_or(U"");
	// }

	Console.writeln(U"[Info] Start with {}"_fmt(romPath.empty() ? U"no ROM file." : romPath));

	Nes::HwFrame nes{};
	const Audio nesAudio{nes.View().GetHw().GetApu().GetStream()};
	nesAudio.play();
	nes.StartRomFile(romPath);

	Gui::UiController gui{};

	while (System::Update())
	{
		nes.ControlFrames();
		gui.Update();
	}

	Gui::SaveGuiIni();
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
