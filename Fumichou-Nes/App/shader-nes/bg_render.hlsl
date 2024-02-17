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

cbuffer CbBgData : register(b1)
{
    uint2 g_patternTableSize;
    uint4 g_nametable[256]; // 4KiB
    uint4 g_palettes[2]; // 32B
}

#define W_256 256
#define H_240 240
#define TILE_8 8

// ((array[address / 16][(address & 16) / 4] >> ((address % 4) * 8)) & 0xFF)
#define FROM_UINT8(array, address) ((array[address >> 4][(address & 0xF) >> 2] >> ((address & 0x3) << 3)) & 0xFF)

float4 PS(s3d::PSInput input) : SV_TARGET
{
    const uint2 screenPos = input.uv * float2(W_256, H_240);
    const uint2 tileCoarse = screenPos / TILE_8;
    const uint2 tileFine = screenPos - tileCoarse * TILE_8;

    const uint addr = tileCoarse.x + tileCoarse.y * 32;

    // const uint16_t addr = 1;
    // const uint addr = 0;
    // const uint tileId = ((g_nametable[addr / 16][(addr % 16) / 4]) >> 8) & 0xFF;
    // const uint tileId = (g_nametable[addr / 16][(addr % 16) / 4] >> ((addr % 4) * 8)) & 0xFF;
    // const uint tileId = (g_nametable[addr >> 4][(addr & 0xF) >> 2] >> ((addr & 0x3) << 3)) & 0xFF;
    const uint tileId = FROM_UINT8(g_nametable, addr);

    const float2 tileUV = float2(uint2(tileId * TILE_8, 0) + tileFine) / g_patternTableSize;
    const float4 tileColor = g_patternTableTexture.Sample(g_sampler0, tileUV);

    float4 outputColor = tileColor;

    // outputColor = g_colors[paletteId];
    // outputColor.r = tileId / 255.0;
    // outputColor.g = tileId / 255.0;

    return outputColor + g_colorAdd;
}
