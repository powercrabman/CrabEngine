cbuffer Transform : register(b0)
{
    row_major float4x4 gWorldMat;
}

cbuffer Camera : register(b1)
{
    row_major float4x4 gViewProj;
    float3 gCameraPos;
}

cbuffer Effect : register(b2)
{
    bool gUseTexture;
}

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    int entityID : ENTITY_ID;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
    int entityID : ENTITY_ID;
};

VSOutput VSmain(VSInput in_input)
{
    VSOutput output;
    output.position = mul(float4(in_input.position, 1.0f), gWorldMat);
    output.position = mul(output.position, gViewProj);
    output.color = in_input.color;
    output.entityID = in_input.entityID;
    output.texCoord = in_input.texCoord;
    return output;
}

Texture2D texDiffuse : register(t0);
SamplerState samLinear : register(s0);
RWTexture2D<uint> entityRWTexture2D : register(u1);

[earlydepthstencil]
float4 PSmain(VSOutput in_input) : SV_TARGET
{
    float4 texColor = { 1.f, 1.f, 1.f, 1.f };
    if (gUseTexture)
    {
        texColor = texDiffuse.Sample(samLinear, in_input.texCoord);
    }

    entityRWTexture2D[uint2(in_input.position.xy)] = in_input.entityID;
    return texColor * in_input.color;
}
