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
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

public: // �����o�֐�
	//���C�g���W���Z�b�g
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightPos_ = lightpos; }
	//���C�g���W���擾
	inline const XMFLOAT3& GetLightPos() { return lightPos_; }
	//���C�g�F���Z�b�g
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	//���C�g�F���擾
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	//���C�g���������W�����Z�b�g
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; }
	//���C�g���������W�����擾
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool active) { active_ = active; }
	//�L���`�F�b�N
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