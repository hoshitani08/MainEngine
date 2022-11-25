#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
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
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// �}�e���A������
	/// </summary>
	/// <returns>�������ꂽ�}�e���A��</returns>
	static Material* Create();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff_;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Material();
	/// <summary>
	/// �萔�o�b�t�@�̎擾
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�ǂݍ��݃f�B���N�g���p�X</param>
	/// <param name="cpuHandle"></param>
	/// <param name="gpuHandle"></param>
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)�̎擾
	/// </summary>
	/// <returns>�V�F�[�_���\�[�X�r���[�̃n���h��(CPU)</returns>
	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV_; }
	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)�̎擾
	/// </summary>
	/// <returns>�V�F�[�_���\�[�X�r���[�̃n���h��(GPU)</returns>
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV_; }

private: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Material()
	{
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �萔�o�b�t�@�̐���
	/// </summary>
	void CreateConstantBuffer();

public: //�����o�ϐ�
	std::string name_; //�}�e���A����
	XMFLOAT3 ambient_; //�A���r�G���g�e���x
	XMFLOAT3 diffuse_; //�f�B�t���[�Y�e���x
	XMFLOAT3 specular_; //�X�y�L�����[�e���x
	float alpha_; //�A���t�@
	std::string textureFilename_; //�e�N�X�`���t�@�C����
};

