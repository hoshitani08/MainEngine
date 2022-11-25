#pragma once

#include <DirectXMath.h>

class CircleShadow
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
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // �����o�֐�
	/// <summary>
	/// ������ݒ�
	/// </summary>
	/// <param name="dir"></param>
	inline void SetDir(const XMVECTOR& dir) { dir_ = DirectX::XMVector3Normalize(dir); }
	/// <summary>
	/// ���C�g�������擾
	/// </summary>
	/// <returns>���C�g����</returns>
	inline const XMVECTOR& GetDir() { return dir_; }
	/// <summary>
	/// �L���X�^�[���W��ݒ�
	/// </summary>
	/// <param name="casterPos">�L���X�^�[���W</param>
	inline void SetCasterPos(const XMFLOAT3& casterPos) { casterPos_ = casterPos; }
	/// <summary>
	/// �L���X�^�[���W���擾
	/// </summary>
	/// <returns>�L���X�^�[���W</returns>
	inline const XMFLOAT3& GetCasterPos() { return casterPos_; }
	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋�����ݒ�
	/// </summary>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	inline void SetDistanceCasterLight(float distanceCasterLight) { distanceCasterLight_ = distanceCasterLight; }
	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋������擾
	/// </summary>
	/// <returns>�L���X�^�[�ƃ��C�g�̋���</returns>
	inline float GetDistanceCasterLight() { return distanceCasterLight_; }
	/// <summary>
	/// ���������W����ݒ�
	/// </summary>
	/// <param name="atten">���������W��</param>
	inline void SetAtten(const XMFLOAT3& atten) { atten_ = atten; }
	/// <summary>
	/// ���������W�����擾
	/// </summary>
	/// <returns>���������W��</returns>
	inline const XMFLOAT3& GetAtten() { return atten_; }
	/// <summary>
	/// �����p�x��ݒ�
	/// </summary>
	/// <param name="factorAngle">x:�����J�n�p�x y:�����I���p�x[radian]</param>
	inline void SetFactorAngle(const XMFLOAT2& factorAngle)
	{
		factorAngleCos_.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		factorAngleCos_.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	/// <summary>
	/// �����p�x���擾
	/// </summary>
	/// <returns>���������W��</returns>
	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos_; }
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
	// �����i�P�ʃx�N�g���j
	XMVECTOR dir_ = { 1,0,0,0 };
	// �L���X�^�[�ƃ��C�g�̋���
	float distanceCasterLight_ = 100.0f;
	// �L���X�^�[���W�i���[���h���W�n�j
	XMFLOAT3 casterPos_ = { 0,0,0 };
	// ���������W��
	XMFLOAT3 atten_ = { 0.5f, 0.6f, 0.0f };
	// �����p�x
	XMFLOAT2 factorAngleCos_ = { 0.2f, 0.5f };
	// �L���t���O
	bool active_ = false;
};