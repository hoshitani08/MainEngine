#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>

#include "PmxMaterial.h"

using namespace DirectX;

class PmxMesh
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

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
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

	struct Surface
	{
		int vertexIndex;
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

public: // �����o�֐�
	~PmxMesh();
	// ���O���擾
	const std::string& GetName() { return name_; }
	// ���O���Z�b�g
	void SetName(const std::string& name) { name_ = name; }
	// ���_�f�[�^�̒ǉ�
	void AddVertex(const VertexPosNormalUv& vertex) { vertices_.emplace_back(vertex); }
	// ���_�C���f�b�N�X�̒ǉ�
	void AddIndex(unsigned short index) { indices_.emplace_back(index); }
	// ���_�f�[�^�̐����擾
	inline size_t GetVertexCount() { return vertices_.size(); }
	// �}�e���A���̎擾
	PmxMaterial* GetMaterial() { return material_; }
	// �}�e���A���̊��蓖��
	void SetMaterial(PmxMaterial* material) { material_ = material; }
	// �o�b�t�@�̐���
	void CreateBuffers();
	// ���_�o�b�t�@�擾
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }
	// �C���f�b�N�X�o�b�t�@�擾
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// ���_�z����擾
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }
	// �C���f�b�N�X�z����擾
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

private: // �����o�ϐ�
	// ���O
	std::string name_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;
	// �}�e���A��
	PmxMaterial* material_ = nullptr;

public:
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices_;
	std::vector<Surface> surfaces;
	// ���f�[�^�z��
	std::vector<Bone> bones;
};