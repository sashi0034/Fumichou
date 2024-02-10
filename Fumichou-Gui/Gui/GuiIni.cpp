#include "stdafx.h"
#include "GuiIni.h"

namespace
{
	constexpr StringView iniPath = U"./gui.ini";

	void loadIniValues(const INI& ini)
	{
		Window::SetPos(Parse<Point>(ini[U"window.pos"]));
	}

	void saveIniValues(INI& ini)
	{
		ini[U"window.pos"] = Window::GetPos();
	}
}

namespace Gui
{
	void LoadGuiIni()
	{
		const INI ini(iniPath);
		if (not ini) return;

		loadIniValues(ini);
	}

	void SaveGuiIni()
	{
		INI ini{};
		saveIniValues(ini);
		ini.save(iniPath);
	}
}
