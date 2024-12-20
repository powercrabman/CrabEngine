
cbuffer Transform : register(b0)
{
    row_major float4x4 gWorldMat;
}

struct VSInput
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VSOutput VSmain(VSInput in_input)
{
    VSOutput output;
    output.position = mul(float4(in_input.position, 1.0f), gWorldMat);
    output.texCoord = in_input.texCoord;
    return output;
}

Texture2D texDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 PSmain(VSOutput in_input) : SV_TARGET
{
    return texDiffuse.Sample(samLinear, in_input.texCoord);
}