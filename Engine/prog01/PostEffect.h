#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PostEffect
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	static const float clearColor[4];
	// ���_��
	static const int vertNum = 4;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; // �F (RGBA)
		float brightnessColor; // ���x
		XMMATRIX mat; // �R�c�ϊ��s��
	};

public: // �����o�֐�
	//�R���X�g���N�^
	PostEffect() = default;
	// ������
	void Initialize(ID3D12Device* device);
	//�`��R�}���h�̔��s
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
// D3D12�f�o�C�X
	ID3D12Device* device = nullptr;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff[2];
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �F (RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���x
	float brightnessColor = 1.0f;
};

