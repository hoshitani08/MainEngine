#pragma once

#include <DirectXMath.h>

class DirectionalLight
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
		XMVECTOR lightv;
		XMFLOAT3 lightColor;
		unsigned int active;
	};

public: // �����o�֐�
	/// <summary>
	/// ���C�g������ݒ�
	/// </summary>
	/// <param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	/// <summary>
	/// ���C�g�������擾
	/// </summary>
	/// <returns>���C�g����</returns>
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	/// <summary>
	/// ���C�g�F��ݒ�
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	/// <summary>
	/// ���C�g�F���擾
	/// </summary>
	/// <returns>���C�g�F</returns>
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
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
	//���C�g�����i�P�ʃx�N�g���j
	XMVECTOR lightDir_ = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightColor_ = { 1,1,1 };
	//�L���t���O
	bool active_ = false;
};