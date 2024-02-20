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

cbuffer CbBgData : register(b2)
{
    struct
    {
        uint2 patternTableSize;
        uint2 padding_0x40;
    } g_ppu;

    uint4 g_nametable[256]; // 4KiB
}

#define W_256 256
#define H_240 240
#define TILE_8 8

// ((array[address / 16][(address & 16) / 4] >> ((address % 4) * 8)) & 0xFF)
#define FROM_UINT8ARRAY(array, address) ((array[(address) >> 4][((address) & 0xF) >> 2] >> (((address) & 0x3) << 3)) & 0xFF)

float4 PS(s3d::PSInput input) : SV_TARGET
{
    const uint2 screenPos = input.uv * float2(W_256, H_240);
    const uint2 tileCoarse = screenPos / TILE_8;
    const uint2 tileFine = screenPos - tileCoarse * TILE_8;

    const uint addr = tileCoarse.x + tileCoarse.y * 32;

    const uint tileId = FROM_UINT8ARRAY(g_nametable, addr);

    const uint attrAddr = 0x3C0 | (addr & 0xC00) | ((addr >> 4) & 0x38) | ((addr >> 2) & 0x7);
    const uint attribute = FROM_UINT8ARRAY(g_nametable, attrAddr);

    const uint shift = ((addr >> 4) & 4) | (addr & 2);
    const uint paletteIdBase = ((attribute >> shift) & 0x3) << 2;

    const float2 tileUV = float2(uint2(tileId * TILE_8, 0) + tileFine) / g_ppu.patternTableSize;
    const float4 tileColor = g_patternTableTexture.Sample(g_sampler0, tileUV);
    const uint paletteIndex = paletteIdBase + tileColor.r + tileColor.g * 2;

    const float4 outputColor = g_paletteColors[FROM_UINT8ARRAY(g_paletteIndexes, paletteIndex)];

    return outputColor;
}
