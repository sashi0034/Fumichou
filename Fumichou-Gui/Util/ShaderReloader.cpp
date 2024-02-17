#include "stdafx.h"
#include "ShaderReloader.h"

#include "ShaderKeys.h"

namespace
{
	// デバッグ用シェーダーリローダ
	class ShaderReloaderAddon : public IAddon
	{
	public:
		DirectoryWatcher m_watcherShaderNes{U"../../Fumichou-Nes/App/shader-nes"};

		~ShaderReloaderAddon() override = default;

		bool update() override
		{
			if (m_watcherShaderNes.retrieveChanges())
			{
				// NESシェーダーリロード

				for (auto& key : Nes::ShaderKeys::AllPS)
				{
					PixelShaderAsset::Unregister(key);
					PixelShaderAsset::Register(key, HLSL(U"{}../{}.hlsl"_fmt(m_watcherShaderNes.directory(), key)));
				}

				for (auto& key : Nes::ShaderKeys::AllVS)
				{
					VertexShaderAsset::Unregister(key);
					VertexShaderAsset::Register(key, HLSL(U"{}../{}.hlsl"_fmt(m_watcherShaderNes.directory(), key)));
				}
			}

			return true;
		}
	};
}

void Util::RegisterShaderReloaderAddon()
{
	Addon::Register<ShaderReloaderAddon>(U"ShaderReloaderAddon");
}
