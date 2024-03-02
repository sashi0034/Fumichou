#include "stdafx.h"
#include "JoyBackground.h"

using namespace Gui;

struct JoyBackground::Impl
{
	void Update(const Size& availableRegion)
	{
		// 上下グラデーション
		Rect{availableRegion}.draw(
			Arg::top = Color{40, 24, 32},
			Arg::bottom = Color{24, 16, 24});

		// ドット模様
		constexpr int step = 32;
		for (int y = 0; y < Scene::Size().y; y += step)
		{
			const double animRate = (Time::GetMillisec() % 1000) / 1000.0;
			for (int x = 0; x < availableRegion.x; x += step)
			{
				Circle{x, y, 1}.draw(Color{
					static_cast<uint8>(180 + 48 * animRate),
					static_cast<uint8>(48 + 64 * animRate),
					static_cast<uint8>(96 + 160 * animRate)
				});
			}
		}
	}
};

namespace Gui
{
	JoyBackground::JoyBackground() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void JoyBackground::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
