#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>

#include "PmxMesh.h"

using namespace DirectX;

// PMX�̃��f���f�[�^�^
struct PMXModelData
{
	static constexpr int NO_DATA_FLAG = -1;

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


	std::vector<Vertex> vertices;
	std::vector<Surface> surfaces;
	std::vector<std::wstring> texturePaths;
	std::vector<Material> materials;
	std::vector<Bone> bones;
};

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

public: // �T�u�N���X
	// texturedModel�V�F�[�_�[�̒萔�o�b�t�@�f�[�^�^
	struct TexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// notTexturedModel�V�F�[�_�[�̒萔�o�b�t�@�f�[�^�^
	struct NotTexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMVECTOR diffuseColor;
		XMVECTOR ambientColor;
	};

public:
	inline size_t getMeshesSize() { return meshes.size(); }
	//inline bool meshHasTexture(int _meshIndex) { return meshes[_meshIndex]->GetpTexture(); }
	bool Initialize(const std::wstring& _filePath, ID3D12Device* const _pDevice);
	//void Draw(DirectX11& _directX11, int _meshIndex, const void* const _pConstantBufferData);

private:
	enum VertexBuffer
	{
		POSITION,
		UV,
		NORMAL,
		VertexBuffer_SIZE,
	};

	//std::array<ComPtr<ID3D12Resource>, VertexBuffer_SIZE> pVertexBuffers;
	ComPtr<ID3D12Resource> pIndexBuffer;
	unsigned mIndexCount;

	PMXModelData allData;
	std::vector<PmxMesh*> meshes;

	HRESULT createTexturedShader(ID3D12Device* const _pDevice, PmxMesh& mesh);
	HRESULT createNotTexturedShader(ID3D12Device* const _pDevice, PmxMesh& mesh);
};