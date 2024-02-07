#include "stdafx.h"
#include "DuiMappingViewer.h"

#include "DuiForward.h"

struct Dui::DuiMappingViewer::Impl
{
	int m_baseAddr{};

	void Update(std::string_view viewName, MappedArrayView mappedArray)
	{
		if (not ImGui::Begin(viewName.data()))
		{
			ImGui::End();
			return;
		}

		// 表示する開始と終了のアドレスを持つスライダーを作成
		constexpr int32 showPageSize = 512;

		if (ImGui::Button("Back"))
		{
			m_baseAddr -= showPageSize;
		}
		ImGui::SameLine();
		if (ImGui::Button("Next"))
		{
			m_baseAddr += showPageSize;
		}

		// ベースアドレスの16進数表示用の一時変数を定義
		char baseAddressStr[8];
		m_baseAddr = Clamp(m_baseAddr, 0, 0xFFFF);
		sprintf_s(baseAddressStr, "%04zx", m_baseAddr);

		// 16進数での入力を受け付けるテキストボックスを作成
		ImGui::PushItemWidth(Size_80);
		if (ImGui::InputText(
			"Base address",
			baseAddressStr,
			sizeof(baseAddressStr),
			ImGuiInputTextFlags_CharsHexadecimal))
		{
			sscanf_s(baseAddressStr, "%zx", &m_baseAddr);
		}

		constexpr int dataSize = Nes::AddrSize_0x10000;

		ImGui::VSliderInt(
			"##v",
			ImVec2(Size_20, ImGui::GetContentRegionAvail().y),
			&m_baseAddr,
			dataSize - showPageSize,
			0,
			"");
		m_baseAddr = std::clamp(m_baseAddr, 0, dataSize - showPageSize);

		ImGui::SameLine();
		ImGui::BeginChild("View", ImVec2(0, 0), true);
		for (size_t addr = m_baseAddr; addr < m_baseAddr + showPageSize; addr += 1)
		{
			if (addr >= dataSize) break;

			// アドレス表示
			ImGui::TextColored(ImColorPurple, "%04zx:", addr);
			ImGui::SameLine();
			const auto desc = std::visit([&](auto&& arg) { return (*arg)[addr].desc; }, mappedArray);
			ImGui::TextColored(ImColorGreen, "%s", desc.data());
		}
		ImGui::EndChild();
		ImGui::End();
	}
};

namespace Dui
{
	DuiMappingViewer::DuiMappingViewer() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void DuiMappingViewer::Update(std::string_view viewName, MappedArrayView mappedArray)
	{
		p_impl->Update(viewName, mappedArray);
	}
}
