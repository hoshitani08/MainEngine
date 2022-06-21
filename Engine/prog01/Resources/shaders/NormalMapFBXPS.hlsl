#include "FBX.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
// �@���}�b�v
Texture2D<float4> g_normalMap : register(t1);
// ���^���b�N�X���[�X�}�b�v�Br�Ƀ��^���b�N�Aa�ɃX���[�X
Texture2D<float4> g_metallicSmoothMap : register(t2);

SamplerState smp : register(s0);      //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

static const float PI = 3.1415926f;

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normalMap.SampleLevel(smp, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torrance���f���̋��ʔ��˂��v�Z
/// </summary>
/// <param name="L">�����Ɍ������x�N�g��</param>
/// <param name="V">���_�Ɍ������x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <param name="metallic">�����x</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
	float microfacet = 0.76f;

	// �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
	// �����x�������قǃt���l�����˂͑傫���Ȃ�
	float f0 = metallic;

	// ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
	float3 H = normalize(L + V);

	// �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = saturate(dot(N, V));

	// D�����x�b�N�}�����z��p���Čv�Z����
	float D = Beckmann(microfacet, NdotH);

	// F����Schlick�ߎ���p���Čv�Z����
	float F = SpcFresnel(f0, VdotH);

	// G�������߂�
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

	// m�������߂�
	float m = PI * NdotV * NdotH;

	// �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
	return max(F * D * G / m, 0.0);
}

/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <remark>
/// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
/// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
/// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
/// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
///
/// </remark>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
/// <param name="roughness">�e���B0�`1�͈̔́B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
	// �f�B�Y�j�[�x�[�X�̃t���l�����˂ɂ��g�U���˂�^�ʖڂɎ�������B
	// �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
	float3 H = normalize(L + V);

	// �e����0.5�ŌŒ�B
	float roughness = 0.5f;

	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

	// �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
	float dotLH = saturate(dot(L, H));

	// �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
	// �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
	float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

	// �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
	float dotNL = saturate(dot(N, L));
	float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

	// �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
	float dotNV = saturate(dot(N, V));
	float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

	// �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
	// ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
	return (FL * FV * energyFactor);
}

float4 main(VSOutput input) : SV_TARGET
{
	// �@�����v�Z
	float3 normal = GetNormal(input.normal, input.tangent, input.biNormal, input.uv);

	// �e��}�b�v���T���v�����O����
	// �A���x�h�J���[�i�g�U���ˌ��j
	float4 albedoColor = tex.Sample(smp, input.uv);

	// �X�y�L�����J���[�̓A���x�h�J���[�Ɠ����ɂ���
	float3 specColor = albedoColor;

	// �����x
	float metallic = g_metallicSmoothMap.Sample(smp, input.uv).r;

	// ���炩��
	float smooth = g_metallicSmoothMap.Sample(smp, input.uv).a;

	// �����Ɍ������ĐL�т�x�N�g�����v�Z����
	float3 toEye = normalize(cameraPos - input.worldpos);

	float3 lig = 0;
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}

		// �t���l�����˂��l�������g�U���˂��v�Z
		float diffuseFromFresnel = CalcDiffuseFromFresnel(
			normal, dirLights[i].lightv, toEye);

		// ���K��Lambert�g�U���˂����߂�
		float NdotL = saturate(dot(normal, dirLights[i].lightv));
		float3 lambertDiffuse = dirLights[i].lightcolor * NdotL / PI;

		// �ŏI�I�Ȋg�U���ˌ����v�Z����
		float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

		// Cook-Torrance���f���̋��ʔ��˗����v�Z����
		float3 spec = CookTorranceSpecular(
			dirLights[i].lightv, toEye, normal, smooth)
			* dirLights[i].lightcolor;

		// �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
		// �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
		spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

		// ���炩����������΁A�g�U���˂͎キ�Ȃ�
		lig += diffuse * (1.0f - smooth) + spec;
	}
	float3 a = { 0.4f, 0.4f, 0.4f };

	// �����ɂ���グ
	lig += a * albedoColor;

	float4 finalColor = 1.0f;
	finalColor.xyz = lig;
	return finalColor;
}