#pragma once

#include <DirectXMath.h>

class PointLight
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		unsigned int active;
	};

public: // �����o�֐�
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
	//���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightPos_ = { 0,0,0 };
	//���C�g�F
	XMFLOAT3 lightColor_ = { 1,1,1 };
	//���C�g���������W��
	XMFLOAT3 lightAtten_ = { 1.0f, 1.0f, 1.0f };
	//�L���t���O
	bool active_ = false;
};