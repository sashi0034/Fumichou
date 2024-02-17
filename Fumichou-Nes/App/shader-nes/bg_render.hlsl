//	Textures
//
Texture2D g_texture0 : register(t0);
SamplerState g_sampler0 : register(s0);

namespace s3d
{
    //
    //	VS Output / PS Input
    //
    struct PSInput
    {
        float4 position : SV_POSITION;
        float4 color : COLOR0;
        float2 uv : TEXCOORD0;
    };
}

//
//	Constant Buffer
//
cbuffer PSConstants2D : register(b0)
{
    float4 g_colorAdd;
    float4 g_sdfParam;
    float4 g_sdfOutlineColor;
    float4 g_sdfShadowColor;
    float4 g_internal;
}

cbuffer CbPaletteColors : register(b1)
{
    float4 g_colors[64];
}

// cbuffer CbPaletteIndex : register(b2)
// {
//     uint4 g_palettes;
// }

float4 PS(s3d::PSInput input) : SV_TARGET
{
    float4 color0 = g_texture0.Sample(g_sampler0, input.uv);

    const int index = (uint)(color0.x) * 2 + (uint)(color0.y);
    const uint paletteId =
        (index == 0) * (input.color.r * 64) +
        (index == 1) * (input.color.g * 64) +
        (index == 2) * (input.color.b * 64) +
        (index == 3) * (input.color.a * 64);
    color0 = g_colors[paletteId];

    return color0 + g_colorAdd;
}
