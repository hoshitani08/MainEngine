#pragma once

#include <DirectXMath.h>

class SpotLight
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public: // �����o�֐�
	// ���C�g�������Z�b�g
	inline void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	// ���C�g�������擾
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	// ���C�g���W���Z�b�g
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightPos_ = lightpos; }
	// ���C�g���W���擾
	inline const XMFLOAT3& GetLightPos() { return lightPos_; }
	// ���C�g�F���Z�b�g
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	// ���C�g�F���擾
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	// ���C�g���������W�����Z�b�g
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; }
	// ���C�g���������W�����擾
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; }
	// ���C�g�����p�x���Z�b�g
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	// ���C�g�����p�x���擾
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos_; }
	// �L���t���O���Z�b�g
	inline void SetActive(bool active) { active_ = active; }
	// �L���`�F�b�N
	inline bool IsActive() { return active_; }

private: // �����o�ϐ�
	// ���C�g�����i�P�ʃx�N�g���j
	XMVECTOR lightDir_ = { 1,0,0,0 };
	// ���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightPos_ = { 0,0,0 };
	// ���C�g�F
	XMFLOAT3 lightColor_ = { 1,1,1 };
	// ���C�g���������W��
	XMFLOAT3 lightAtten_ = { 1.0f, 1.0f, 1.0f };
	// ���C�g�����p�x
	XMFLOAT2 lightFactorAngleCos_ = { 0.2f, 0.5f };
	// �L���t���O
	bool active_ = false;
};