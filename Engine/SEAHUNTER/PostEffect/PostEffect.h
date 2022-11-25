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
	static const float CLEAR_COLOR[4];
	// ���_��
	static const int VERT_NUM = 4;

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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect() = default;
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
// D3D12�f�o�C�X
	ID3D12Device* device_ = nullptr;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff_[2];
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff_;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// �F (RGBA)
	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// ���x
	float brightnessColor_ = 1.0f;
};

