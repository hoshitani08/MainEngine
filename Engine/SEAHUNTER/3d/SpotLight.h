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
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		float pad3;
		XMFLOAT2 lightFactorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // �����o�֐�
	/// <summary>
	/// ���C�g������ݒ�
	/// </summary>
	/// <param name="lightdir">���C�g����</param>
	inline void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	/// <summary>
	/// ���C�g�������擾
	/// </summary>
	/// <returns>���C�g����</returns>
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	/// <summary>
	/// ���C�g���W��ݒ�
	/// </summary>
	/// <param name="lightpos">���C�g���W</param>
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightPos_ = lightpos; }
	/// <summary>
	/// ���C�g���W���擾
	/// </summary>
	/// <returns>���C�g���W</returns>
	inline const XMFLOAT3& GetLightPos() { return lightPos_; }
	/// <summary>
	/// ���C�g�F��ݒ�
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	/// <summary>
	/// ���C�g�F���擾
	/// </summary>
	/// <returns>���C�g�F</returns>
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	/// <summary>
	/// ���C�g���������W����ݒ�
	/// </summary>
	/// <param name="lightAtten">���C�g���������W��</param>
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; }
	/// <summary>
	/// ���C�g���������W�����擾
	/// </summary>
	/// <returns>���C�g���������W��</returns>
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; }
	/// <summary>
	/// ���C�g�����p�x��ݒ�
	/// </summary>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x[radian]</param>
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	/// <summary>
	/// ���C�g�����p�x���擾
	/// </summary>
	/// <returns>���C�g�����p�x</returns>
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos_; }
	/// <summary>
	/// �L���t���O��ݒ�
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { active_ = active; }
	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
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