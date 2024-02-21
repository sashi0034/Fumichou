//
//	Textures
//
Texture2D g_patternTableTexture : register(t0);
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
    float4 g_paletteColors[64];
    uint4 g_paletteIndexes[2]; // 32B
}

#define W_256 256
#define H_240 240
#define TILE_8 8

// ((array[address / 16][(address & 16) / 4] >> ((address % 4) * 8)) & 0xFF)
#define FROM_UINT8ARRAY(array, address) ((array[(address) >> 4][((address) & 0xF) >> 2] >> (((address) & 0x3) << 3)) & 0xFF)

float4 PS(s3d::PSInput input) : SV_TARGET
{
    const uint paletteIdBase = input.color[0];

    const float4 tileColor = g_patternTableTexture.Sample(g_sampler0, input.uv);
    const uint paletteOffset = tileColor.r + tileColor.g * 2;
    const uint paletteIndex = paletteIdBase + paletteOffset;

    float4 outputColor = g_paletteColors[FROM_UINT8ARRAY(g_paletteIndexes, paletteIndex)];

    // Pixels in palette 0 are hidden
    outputColor.a = paletteOffset;

    return outputColor;
}
