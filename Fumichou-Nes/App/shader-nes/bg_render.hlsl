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
    uint4 g_patternTableSize;
    uint4 g_tilePageOffsets; // 4 page offsets of 64 tiles
    uint4 g_scrollX[30];
    uint4 g_scrollY[30];
    uint4 g_nametable[256]; // 4KiB
}

#define W_256 256
#define H_240 240
#define TILE_8 8

// ((array[address / 16][(address % 16) / 4] >> ((address % 4) * 8)) & 0xFF)
#define FROM_UINT8ARRAY(array, address) ((array[(address) >> 4][((address) & 0xF) >> 2] >> (((address) & 0x3) << 3)) & 0xFF)

// ((array[address / 8][(address % 8) / 2] >> ((address % 2) * 16)) & 0xFF)
#define FROM_UINT16ARRAY(array, address) ((array[(address) >> 3][((address) & 0x7) >> 1] >> (((address) & 0x1) << 4)) & 0xFFFF)

float4 PS(s3d::PSInput input) : SV_TARGET
{
    const uint2 uv = input.uv * float2(W_256, H_240);
    const uint2 scrollPos = uint2(FROM_UINT16ARRAY(g_scrollX, uv.y), FROM_UINT16ARRAY(g_scrollY, uv.y));
    const uint2 screenPos0 = scrollPos + uv;
    const uint2 screenPos = screenPos0 % (2 * uint2(W_256, H_240));
    const uint2 tileCoarse = screenPos / TILE_8;
    const uint2 tileFine = screenPos - tileCoarse * TILE_8;

    const uint crossPageX = ((tileCoarse.x) >> 5) * (0x400 - 32); // Correct if X is greater than 32
    const uint crossPageY = ((tileCoarse.y + 2) >> 5) * (0x400 + 64); // Correct if Y is greater than 30
    const uint addr = tileCoarse.x + tileCoarse.y * 32 + crossPageX + crossPageY;

    const uint tileId0 = FROM_UINT8ARRAY(g_nametable, addr); // [0, 512)
    const uint tileId = g_tilePageOffsets[tileId0 >> 6] | (tileId0 & 0x3F);

    const uint attrAddr = 0x3C0 | (addr & 0xC00) | ((addr >> 4) & 0x38) | ((addr >> 2) & 0x7);
    const uint attribute = FROM_UINT8ARRAY(g_nametable, attrAddr);

    const uint shift = ((addr >> 4) & 4) | (addr & 2);
    const uint paletteIdBase = ((attribute >> shift) & 0x3) << 2;

    const float2 tileUV = float2(uint2(tileId * TILE_8, 0) + tileFine) / g_patternTableSize.xy;
    const float4 tileColor = g_patternTableTexture.Sample(g_sampler0, tileUV);
    const uint paletteOffset = (uint(tileColor.r) | (uint(tileColor.g) << 1));
    const uint paletteIndex = paletteIdBase + paletteOffset;

    float4 outputColor = g_paletteColors[FROM_UINT8ARRAY(g_paletteIndexes, paletteIndex)];
    outputColor.a = paletteOffset;

    return outputColor;
}
