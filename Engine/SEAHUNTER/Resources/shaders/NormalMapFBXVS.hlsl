#include "FBX.hlsli"

SkinOutput ComputeSkin(VSInput input)
{
	//�[���N���A
	SkinOutput output = (SkinOutput)0;

	uint iBone;//�v�Z����{�[���ԍ�
	float weight;//�{�[���E�F�C�g(�d��)
	matrix m;//�X�L�j���O�s��

	//�{�[��0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	output.tangent += weight * mul((float3x3)m, input.tangent);
	output.biNormal += weight * mul((float3x3)m, input.biNormal);

	//�{�[��1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	output.tangent += weight * mul((float3x3)m, input.tangent);
	output.biNormal += weight * mul((float3x3)m, input.biNormal);

	//�{�[��2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	output.tangent += weight * mul((float3x3)m, input.tangent);
	output.biNormal += weight * mul((float3x3)m, input.biNormal);

	//�{�[��3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	output.tangent += weight * mul((float3x3)m, input.tangent);
	output.biNormal += weight * mul((float3x3)m, input.biNormal);

	return output;
}

VSOutput main(VSInput input)
{
	//�X�L�j���O�v�Z
	SkinOutput skinned = ComputeSkin(input);
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//�s��ɂ����W�ϊ�
	output.svpos = mul(mul(viewproj, world), skinned.pos);
	//���[���h���W�����̃X�e�[�W�ɓn��
	output.worldpos = mul(world, skinned.pos).xyz;
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;

	float4 wtangent = normalize(mul(world, float4(skinned.tangent, 0)));
	float4 wbiNormal = normalize(mul(world, float4(skinned.biNormal, 0)));

	output.tangent = wtangent.xyz;
	output.biNormal = wbiNormal.xyz;
	//���͒l�����̂܂܎��̃X�e�[�W�ɓn��
	output.uv = input.uv;

	return output;
}