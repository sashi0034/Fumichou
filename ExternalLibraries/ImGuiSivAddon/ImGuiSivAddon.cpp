#define NO_S3D_USING
#include <Siv3D.hpp>
#include "imgui/imgui.h"
#include "imgui_impl_s3d.h"
#include "ImGuiSivAddon.hpp"

struct ImGuiSivAddon::Impl
{
	bool m_firstFrame{true};
};

ImGuiSivAddon::ImGuiSivAddon():
	p_impl(std::make_shared<Impl>())
{
}

/// @brief アドオンの登録時の初期化処理を記述します。
/// @remark この関数が false を返すとアドオンの登録は失敗します。
/// @return アドオンの初期化に成功した場合 true, それ以外の場合は false

bool ImGuiSivAddon::init()
{
	ImGui::CreateContext();
	ImGui_ImplS3d_Init();

	return true;
}

/// @brief アドオンの毎フレームの更新処理を記述します。
/// @remark この関数が false を返すと `System::Update()` は false を返します。
/// @return アドオンの更新に成功した場合 true, それ以外の場合は false

bool ImGuiSivAddon::update()
{
	ImGui_ImplS3d_NewFrame();
	ImGui::NewFrame();

	p_impl->m_firstFrame = false;
	return true;
}

/// @brief アドオンの毎フレームの描画処理を記述します。

void ImGuiSivAddon::draw() const
{
	if (p_impl->m_firstFrame)
	{
		return;
	}

	ImGui::Render();
	ImGui_ImplS3d_RenderDrawData(::ImGui::GetDrawData());
}

ImGuiSivAddon::~ImGuiSivAddon()
{
	ImGui_ImplS3d_Shutdown();
}

struct ImS3dTexture::Impl
{
	explicit Impl(const Texture& texture): m_texture(texture)
	{
		m_id = ImGui_ImplS3d_RegisterTexture(texture);
	}

	~Impl()
	{
		ImGui_ImplS3d_UnregisterTexture(m_texture);
	}

	Texture m_texture;
	ImTextureID m_id;
};

ImS3dTexture::ImS3dTexture(const Texture& texture) : m_impl(std::make_shared<Impl>(texture))
{
}

Texture& ImS3dTexture::GetTexture() const
{
	return m_impl->m_texture;
}

Optional<ImTextureID> ImS3dTexture::GetId() const
{
	if (m_impl->m_id == nullptr) return none;
	return m_impl->m_id;
}
