//	Textures
//
Texture2D g_texture0 : register(t0);
SamplerState g_sampler0 : register(s0);

namespace s3d
{
    float4 Transform2D(float2 pos, float2x4 t)
    {
        return float4((t._13_14 + (pos.x * t._11_12) + (pos.y * t._21_22)), t._23_24);
    }
}

//
//	VS Output / PS Input
//
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

//
//	Constant Buffer
//
cbuffer VSConstants2D : register(b0)
{
    row_major float2x4 g_transform;
    float4 g_colorMul;
}

cbuffer CbPaletteColors : register(b1)
{
    float4 g_colors[64];
}

cbuffer CbBgData : register(b2)
{
    uint g_patternTableLength;
    uint2 g_tileSize;
    uint4 g_nametable[256]; // 4KiB
    uint4 g_palettes[2]; // 32B
}

float4 PS(PSInput input) : SV_TARGET
{
    float4 color0 = g_texture0.Sample(g_sampler0, input.uv);

    const uint index = (uint)(color0.x) * 2 + (uint)(color0.y); // TODO inputからベース足す
    const uint paletteIndex = (g_palettes[index << 4][(index << 2) & 0x3] << ((index & 0x3) >> 2)) & 0xFF;
    color0 = g_colors[paletteIndex];
    color0.b = 1;

    return color0;
}

float2 toUV(uint mod6)
{
    if (mod6 == 0)
    {
        return float2(0, 0);
    }
    else if (mod6 == 1)
    {
        return float2(1, 0);
    }
    else if (mod6 == 2)
    {
        return float2(0, 1);
    }
    else if (mod6 == 3)
    {
        return float2(0, 1);
    }
    else if (mod6 == 4)
    {
        return float2(1, 0);
    }
    else
    {
        return float2(1, 1);
    }
}

PSInput VS(uint id: SV_VERTEXID)
{
    PSInput output;

    const uint quadId = id / 6;
    const uint mod6 = id % 6;

    const uint tileX = quadId / g_tileSize.y;
    const uint tileY = quadId % g_tileSize.y;
    const uint addr = tileX + tileY * 32;

    const uint tileId = (g_nametable[addr << 4][(addr << 2) & 0x3] << ((addr & 0x3) >> 2)) & 0xFF;

    const uint attrIndex = 0x3C0 | (addr & 0xC00) | ((addr >> 4) & 0x38) | ((addr >> 2) & 0x7);
    const uint attribute = (g_nametable[attrIndex << 4][(attrIndex << 2) & 0x3] << ((attrIndex & 0x3) >> 2)) & 0xFF;

    const uint shift = ((addr >> 4) & 4) | (addr & 2);
    const uint paletteIndex = ((attribute >> shift) & 0x3) << 2; // TODO

    const float2 uvOffset = toUV(mod6);
    output.uv = float2(tileId / g_patternTableLength, 0);
    output.uv += float2(uvOffset.x / g_patternTableLength, 0);

    float2 pos = 16 * float2(tileX, tileY);

    const float2 size = float2(1, 1);
    pos += lerp(float2(0, 0), size, uvOffset);

    output.position = s3d::Transform2D(pos, g_transform);
    output.color = g_colorMul;

    return output;
}
