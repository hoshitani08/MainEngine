#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

// �m�[�h
struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

//�e�N�X�`���f�[�^
struct TextureData
{
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metaData = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};
	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRV��GPU�n���h��
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxLoader;

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:// �萔
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;
	//�e�N�X�`���̍ő吔
	static const int MAX_TEXTURES = 4;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;              //xyz���W
		DirectX::XMFLOAT3 normal;           //�@���x�N�g��
		DirectX::XMFLOAT2 uv;               //uv���W
		DirectX::XMFLOAT3 tangent;          //�ڃx�N�g��
		DirectX::XMFLOAT3 binormal;         //�]�x�N�g��
		UINT boneIndex[MAX_BONE_INDICES];   //�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES]; //�{�[���d��
	};

	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

public:
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void CreateBuffers(ID3D12Device* device);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// ���f���̕ό`�s��擾
	/// </summary>
	/// <returns>�ό`�s��</returns>
	const XMMATRIX& GetModelTransform() { return meshNode_->globalTransform; }
	/// <summary>
	/// �e�N�X�`���o�b�t�@����
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="srvIndex"></param>
	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);
	/// <summary>
	/// �{�[���̎擾
	/// </summary>
	/// <returns>�{�[��</returns>
	std::vector<Bone>& GetBones() { return bones_; }
	/// <summary>
	/// FBX�V�[���̎擾�f�X�g���N�^
	/// </summary>
	/// <returns>FBX�V�[��</returns>
	FbxScene* GetFbxScene() { return fbxScene_; }
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FbxModel();

private:
	//���f����
	std::string name_;
	//�m�[�h�z��
	std::vector<Node> nodes_;
	//�{�[���z��
	std::vector<Bone> bones_;
	//FBX�V�[��
	FbxScene* fbxScene_ = nullptr;
	//���b�V�������m�[�h
	Node* meshNode_ = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices_;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;
	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metaData_ = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg_ = {};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff_;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff_;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
	//�x�[�X�e�N�X�`��
	TextureData baseTexture_;
	//���^���l�X�e�N�X�`��
	TextureData metalnessTexture_;
	//�@���e�N�X�`��
	TextureData normalTexture_;
};