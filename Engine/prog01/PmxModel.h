#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <memory>

#include "PmxMesh.h"

using namespace DirectX;

class PmxModel
{
public:
	//�t�����h�N���X
	friend class PmxLoader;

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	// std::���ȗ�
	using string = std::string;
	using wstring = std::wstring;

private: // �萔
	static constexpr int NO_DATA_FLAG = -1;

public: // �T�u�N���X
	struct Vertex
	{
		// ���_���W
		XMFLOAT3 position;
		// �@��
		XMFLOAT3 normal;
		// UV���W
		XMFLOAT2 uv;
		// �ǉ�UV���W
		std::vector<XMFLOAT4> additionalUV;

		// �{�[���E�F�C�g
		struct Weight
		{
			enum Type
			{
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
			};

			Type type;
			int born1;
			int	born2;
			int	born3;
			int	born4;
			float weight1;
			float weight2;
			float weight3;
			float weight4;
			XMFLOAT3 c;
			XMFLOAT3 r0;
			XMFLOAT3 r1;
		} weight;

		// �G�b�W�{��
		float edgeMagnif;
	};

	struct Surface
	{
		int vertexIndex;
	};

	struct Material
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 specular;
		float specularity;
		XMFLOAT3 ambient;

		int colorMapTextureIndex;
		int toonTextureIndex;
		// (�X�t�B�A�e�N�X�`���͔�Ή�)

		// �ގ����Ƃ̒��_��
		int vertexNum;
	};

	struct Bone
	{
		// �{�[����
		std::wstring name;
		// English version
		std::string nameEnglish;
		XMFLOAT3 position;
		int parentIndex;
		int transformationLevel;
		unsigned short flag;
		XMFLOAT3 coordOffset;
		int childrenIndex;
		int impartParentIndex;
		float impartRate;
		// �Œ莲�����x�N�g��
		XMFLOAT3 fixedAxis;
		// ���[�J����X�������x�N�g��
		XMFLOAT3 localAxisX;
		// ���[�J����Z�������x�N�g��
		XMFLOAT3 localAxisZ;
		int externalParentKey;
		int ikTargetIndex;
		int ikLoopCount;
		float ikUnitAngle;
		struct IKLink
		{
			int index;
			bool existAngleLimited;
			XMFLOAT3 limitAngleMin;
			XMFLOAT3 limitAngleMax;
		};
		std::vector<IKLink> ikLinks;
	};

	// ���b�V��
	struct Mesh
	{
		unsigned vertexNum;
		ComPtr<ID3D12Resource> texbuff;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 specularColor;
		float specularity;
		XMFLOAT3 ambientColor;
	};

public:
	inline size_t getMeshesSize() { return meshes.size(); }
	//inline bool meshHasTexture(int _meshIndex) { return meshes[_meshIndex]->GetpTexture(); }
	bool Initialize();
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

	static std::unique_ptr<PmxModel> CreateFromObject(const std::wstring& text, bool smoothing = false);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

private:
	//unsigned mIndexCount;
	std::vector<Mesh> meshes;

private: //FBX����K�v���Ǝv���Ĉ��������Ă�������
	//���_�f�[�^�z��
	std::vector<Vertex> vertices;
	std::vector<Surface> surfaces;
	std::vector<wstring> texturePaths;
	std::vector<Material> materials;
	std::vector<Bone> bones;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//SRV��GPU�n���h��
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};