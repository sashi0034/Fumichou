#pragma once
#include "imgui/imgui.h"

IMGUI_IMPL_API bool ImGui_ImplS3d_Init();
IMGUI_IMPL_API void ImGui_ImplS3d_Shutdown();
IMGUI_IMPL_API void ImGui_ImplS3d_NewFrame();
IMGUI_IMPL_API void ImGui_ImplS3d_RenderDrawData(ImDrawData* draw_data);

IMGUI_IMPL_API bool ImGui_ImplS3d_RegisterFont(StringView);
IMGUI_IMPL_API ImTextureID ImGui_ImplS3d_RegisterTexture(const s3d::Texture& tex);
IMGUI_IMPL_API void ImGui_ImplS3d_UnregisterTexture(const s3d::Texture& tex);
IMGUI_IMPL_API s3d::Texture ImGui_ImplS3d_GetTexture(ImTextureID id);
