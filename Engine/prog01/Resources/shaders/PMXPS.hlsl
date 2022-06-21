#include "PMX.hlsli"

Texture2D<float4> modelTexture : register(t0);
SamplerState modelSampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color;
    float4 textureColor = modelTexture.Sample(modelSampler, input.uv);

    color = textureColor;

    return color;
}