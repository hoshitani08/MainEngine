#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 1番スロットに設定されたテクスチャ
Texture2D<float> tex2 : register(t2);   // 2番スロットに設定されたテクスチャ
SamplerState smp : register(s0);        // 0番スロットに設定されたサンプラー

static const int BLUR_SAMPLE_COUNT = 9;

static const float2 BLUR_KERNEL[BLUR_SAMPLE_COUNT] =
{
	float2(-1.0f / 1280, -1.0f / 720),
	float2(0.0f / 1280, -1.0f / 720),
	float2(1.0f / 1280, -1.0f / 720),
	float2(-1.0f / 1280,  0.0f / 720),
	float2(0.0f / 1280,  0.0f / 720),
	float2(1.0f / 1280,  0.0f / 720),
	float2(-1.0f / 1280,  1.0f / 720),
	float2(0.0f / 1280,  1.0f / 720),
	float2(1.0f / 1280,  1.0f / 720),
};

float4 Reverse(float4 baseColor)
{
	float3 maxColor = { 1, 1, 1 };

	baseColor.rgb = maxColor - baseColor.rgb;

	return baseColor;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sigma, float shiftWidth)
{
	float totalWeight = 0;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -sigma * 2; py <= sigma * 2; py += shiftWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += shiftWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			col += tex0.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;
	return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	//float4 texColor = tex0.Sample(smp, input.uv);
	////swssfloat depth = tex2.Sample(smp, input.uv);
	//float _Depth = 1 - 0.015f;
	//float _NWidth = 0.015f;
	//float _FWidth = 0.02f;
	//float depth = tex2.Sample(smp, input.uv);
	//float far = smoothstep(_NWidth / 2, _NWidth / 2 + _FWidth / 2, abs(depth - _Depth));
	//float4 color = { texColor.rgb, (1 - far) };

	//float4 finalColor = 0.0f;

	/*for (int j = 0; j < BLUR_SAMPLE_COUNT; j++)
	{
		finalColor += tex0.Sample(smp, input.uv + BLUR_KERNEL[j]);
	}*/

	//finalColor += float4(texColor.rgb * brightnessColor, 1);
	//色反転
	//finalColor = Reverse(finalColor);

	//// 最後にサンプリング数で割る
	//finalColor.rgb /= BLUR_SAMPLE_COUNT;
	//finalColor.a = 1;

	/*float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = colortex1;
	}*/

	//return texColor;

	float _Depth = 1 - 0.02f;
	float _NWidth = 0.02f;
	float _FWidth = 0.02f;
	float depth = tex2.Sample(smp, input.uv);
	float near = smoothstep(0, _NWidth / 2, abs(depth - _Depth));
	float far = smoothstep(_NWidth / 2, _NWidth / 2 + _FWidth / 2, abs(depth - _Depth));
	float4 pintColor = tex0.Sample(smp, input.uv);
	float4 nearColor = tex0.Sample(smp, input.uv);
	float4 farColor = GaussianBlur(input.uv, 0.003f, 0.0005f);
	float4 color = { farColor.rgb, (1 - far) };

	return (1 - near) * pintColor + near * ((1 - far) * nearColor + far * color);

	//return color;
}