#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔
	// ���s�����̐�
	static const int DIR_LIGHT_NUM = 3;
	// �_�����̐�
	static const int POINT_LIGHT_NUM = 3;
	// �X�|�b�g���C�g�̐�
	static const int SPOT_LIGHT_NUM = 3;
	// �ۉe�̐�
	static const int CIRCLE_SHADOW_NUM = 1;

public: // �T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
		// �_�����p
		PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
		// �X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SPOT_LIGHT_NUM];
		// �ۉe�p
		CircleShadow::ConstBufferData circleShadows[CIRCLE_SHADOW_NUM];
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<LightGroup> Create();

private: // �ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device_;

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LightGroup();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	/// <param name="rootParameterIndex">���[�g�n���h��</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();
	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();
	/// <summary>
	/// �����̃��C�g�F��ݒ�
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const XMFLOAT3& color);
	/// <summary>
	/// ���s�����̗L���t���O��ݒ�
	/// </summary>
	/// <param name="index"></param>
	/// <param name="active"></param>
	void SetDirLightActive(int index, bool active);
	/// <summary>
	/// ���s�����̃��C�g������ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// ���s�����̃��C�g�F��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// �_�����̗L���t���O��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetPointLightActive(int index, bool active);
	/// <summary>
	/// �_�����̃��C�g���W��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);
	/// <summary>
	/// �_�����̃��C�g�F��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// �_�����̃��C�g���������W����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// �X�|�b�g���C�g�̗L���t���O��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetSpotLightActive(int index, bool active);
	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g������ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���W��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�F��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���������W����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�����p�x��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x[radian]</param>
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
	/// <summary>
	/// �ۉe�̗L���t���O��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetCircleShadowActive(int index, bool active);
	/// <summary>
	/// �ۉe�̃L���X�^�[���W��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="casterPos">�L���X�^�[���W</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);
	/// <summary>
	/// �ۉe�̕�����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">����</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// �ۉe�̃L���X�^�[�ƃ��C�g�̋�����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	/// <summary>
	/// �ۉe�̋��������W����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���������W��</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// �ۉe�̌����p�x��ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightFactorAngle">x:�����J�n�p�x y:�����I���p�x[radian]</param>
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

private: // �����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	//�����̐F
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights_[DIR_LIGHT_NUM];
	//�_�����̔z��
	PointLight pointLights_[POINT_LIGHT_NUM];
	// �X�|�b�g���C�g�̔z��
	SpotLight spotLights_[SPOT_LIGHT_NUM];
	// �ۉe�̔z��
	CircleShadow circleShadows_[CIRCLE_SHADOW_NUM];
	//�_�[�e�B�t���O
	bool dirty_ = false;
};