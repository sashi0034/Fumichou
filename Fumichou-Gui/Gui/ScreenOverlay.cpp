#include "stdafx.h"
#include "ScreenOverlay.h"

using namespace Gui;

namespace
{
	constexpr int modeCount_2 = 2;
}

struct ScreenOverlay::Impl
{
	void Update(const update_args& args)
	{
		if (not args.screenRect.intersects(Cursor::Pos())) return;

		RectF(Arg::topCenter = args.screenRect.topCenter(), SizeF(args.screenRect.w, 80)).draw(
			Arg::top = ColorF{0.1, 0.5},
			Arg::bottom = ColorF{0.1, 0.0});

		if (MouseL.down())
		{
			// TODO: アイコン表示
			args.modeIndex.get() = (args.modeIndex + 1) % modeCount_2;
		}
	}
};

namespace Gui
{
	ScreenOverlay::ScreenOverlay() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void ScreenOverlay::Update(const update_args& args)
	{
		p_impl->Update(args);
	}
}
