#include "stdafx.h"
#include "DuiScene.h"

#include "Utils/Utils.h"

using namespace Utils;

struct Dui::DuiScene::Impl
{
	ImS3dTexture m_screen{Texture(U"🤖"_emoji)};

	void Update(Nes::HwFrame& nes)
	{
		updateMainDisplay(nes);
	}

private:
	void updateMainDisplay(Nes::HwFrame& nes) const
	{
		ImGui::Begin("Main display");

		if (const auto id = m_screen.GetId())
		{
			ImGui::Image(id.value(), ImVec2{Nes::DisplayWidth_256, Nes::DisplayHeight_240});
		}

		ImGui::End();
	}
};

namespace Dui
{
	DuiScene::DuiScene() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void DuiScene::Update(Nes::HwFrame& nes)
	{
		p_impl->Update(nes);
	}
}
