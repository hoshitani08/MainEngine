#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class PmxMaterial
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //�A���r�G���g�W��
		float pad1; //�p�f�B���O
		XMFLOAT3 diffuse; //�f�B�t���[�Y�W��
		float pad2; //�p�f�B���O
		XMFLOAT3 specular; //�X�y�L�����[�W��
		float alpha; //�A���t�@
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);
	//�}�e���A������
	static PmxMaterial* Create();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff_;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

public: // �����o�֐�

	~PmxMaterial();
	// �萔�o�b�t�@�̎擾
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }
	// �e�N�X�`���ǂݍ���
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	// �X�V
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV_; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV_; }

private: // �����o�֐�
	//�R���X�g���N�^
	PmxMaterial()
	{
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}
	// ������
	void Initialize();
	// �萔�o�b�t�@�̐���
	void CreateConstantBuffer();

public: //�����o�ϐ�
	std::string name_; ////�e�N�X�`���t�@�C����
	XMFLOAT3 ambient_; //�A���r�G���g�e���x
	XMFLOAT3 diffuse_; //�f�B�t���[�Y�e���x
	XMFLOAT3 specular_; //�X�y�L�����[�e���x
	float alpha_; //�A���t�@

	int colorMapTextureIndex;
	int toonTextureIndex;
	int vertexNum;
};

