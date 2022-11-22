#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <unordered_map>

#include "Material.h"

class Mesh
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
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;

public: // �����o�֐�

	~Mesh();
	// ���O���擾
	const std::string& GetName() { return name_; }
	// ���O���Z�b�g
	void SetName(const std::string& name);
	// ���_�f�[�^�̒ǉ�
	void AddVertex(const VertexPosNormalUv& vertex);
	// ���_�C���f�b�N�X�̒ǉ�
	void AddIndex(unsigned short index);
	// ���_�f�[�^�̐����擾
	inline size_t GetVertexCount() { return vertices_.size(); }
	// �}�e���A���̎擾
	Material* GetMaterial() { return material_; }
	// �}�e���A���̊��蓖��
	void SetMaterial(Material* material);
	// �o�b�t�@�̐���
	void CreateBuffers();
	// ���_�o�b�t�@�擾
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }
	// �C���f�b�N�X�o�b�t�@�擾
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�G�b�W�������f�[�^�̒ǉ�
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	//���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVetexNormals();
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
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices_;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices_;
	// �}�e���A��
	Material* material_ = nullptr;
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
};