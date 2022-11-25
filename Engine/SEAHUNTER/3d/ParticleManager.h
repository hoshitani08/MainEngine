#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <memory>

#include "Camera.h"

class ParticleManager
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
	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale; // �X�P�[��
		XMFLOAT4 color; // �F(RGBA)
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat; // �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard; // �r���{�[�h�s��
	};

	//�p�[�e�B�N��1��
	class Particle
	{
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		//�����x
		XMFLOAT3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int numFrame = 0;
		// �X�P�[��
		float scale = 1.0f;
		// �����l
		float startScale = 1.0f;
		// �ŏI�l
		float endScale = 0.0f;
		// �F(RGBA)
		XMFLOAT4 color = { 1, 1, 1, 1 };
		// �F(RGBA)�����l
		XMFLOAT4 startColor = {};
		// �F(RGBA)�ŏI�l
		XMFLOAT4 endColor = {};
	};

private: // �萔
	static const int VERTEX_COUNT = 65536; // ���_��

public: // �ÓI�����o�֐�
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="camera">�J����</param>
	/// <param name="fName">�t�@�C����</param>
	/// <returns></returns>
	static std::unique_ptr<ParticleManager> Create(ID3D12Device* device, Camera* camera, std::wstring fName = L"effect1");

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ParticleManager();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	void Initialize(std::wstring fName);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="startScale">�����l�̔{��</param>
	/// <param name="endScale">�ŏI�l�̔{��</param>
	/// <param name="startColor">�F(RGBA)�����l</param>
	/// <param name="endColor">�F(RGBA)�ŏI�l</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		float startScale, float endScale, XMFLOAT4 startColor, XMFLOAT4 endColor);
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	void InitializeDescriptorHeap();
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="fName"></param>
	void LoadTexture(std::wstring fName);
	/// <summary>
	/// ���f���쐬
	/// </summary>
	void CreateModel();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	ID3D12Device* device_ = nullptr;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize_ = 0u;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	//�p�[�e�B�N���z��
	std::forward_list<Particle> particles_;
	// �J����
	Camera* camera_ = nullptr;

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="camera">�J����</param>
	ParticleManager(ID3D12Device* device, Camera* camera);
};