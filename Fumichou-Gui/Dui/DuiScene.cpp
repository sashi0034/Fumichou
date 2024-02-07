#include "stdafx.h"
#include "DuiScene.h"

#include "DuiMappingViewer.h"
#include "DuiMemoryViewer.h"
#include "Utils/Utils.h"

using namespace Utils;

struct Dui::DuiScene::Impl
{
	ImS3dTexture m_display{Texture(U"🫥"_emoji)};

	DuiMemoryViewer m_internalRamViewer{};
	DuiMemoryViewer m_prgRomViewer{};
	DuiMemoryViewer m_chrRomViewer{};

	DuiMappingViewer m_cpuReadBus{};
	DuiMappingViewer m_cpuWriteBus{};
	DuiMappingViewer m_ppuReadBus{};
	DuiMappingViewer m_ppuWriteBus{};

	void Update(Nes::HwFrame& nes)
	{
		updateDocking();
		updateMainDisplay(nes);

		m_internalRamViewer.Update("Internal RAM", nes.GetEnv().GetRam().GetInternalRam());
		m_prgRomViewer.Update("PRG-ROM", nes.GetEnv().GetCartridge().GetRomData().GetPrg());
		m_chrRomViewer.Update("CHR-ROM", nes.GetEnv().GetCartridge().GetRomData().GetChr());

		m_cpuReadBus.Update("CPU R", &nes.GetEnv().GetMmu().GetCpuRead());
		m_cpuWriteBus.Update("CPU W", &nes.GetEnv().GetMmu().GetCpuWrite());
		m_ppuReadBus.Update("PPU R", &nes.GetEnv().GetMmu().GetPpuRead());
		m_ppuWriteBus.Update("PPU W", &nes.GetEnv().GetMmu().GetPpuWrite());
	}

private:
	void updateDocking() const
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::Begin("Docking fullscreen", nullptr,
		             ImGuiWindowFlags_NoTitleBar |
		             ImGuiWindowFlags_NoResize |
		             ImGuiWindowFlags_NoMove |
		             ImGuiWindowFlags_NoCollapse |
		             ImGuiWindowFlags_NoBringToFrontOnFocus |
		             ImGuiWindowFlags_NoNavFocus);

		ImGui::DockSpace(ImGui::GetID("Main docking"), ImVec2(0.0f, 0.0f),
		                 Window::GetState().fullscreen
			                 ? ImGuiDockNodeFlags_None
			                 : ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
	}

	void updateMainDisplay(Nes::HwFrame& nes) const
	{
		ImGui::Begin("Main display");

		if (const auto id = m_display.GetId())
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
		constexpr int defaultFontSize = 24;
		ImGui::GetIO().Fonts->AddFontFromFileTTF("asset/font/DotGothic16/DotGothic16-Regular.ttf", defaultFontSize);
	}

	void DuiScene::Update(Nes::HwFrame& nes)
	{
		p_impl->Update(nes);
	}
}
