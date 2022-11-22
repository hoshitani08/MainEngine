#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Mesh.h"

class Model
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

	static std::unique_ptr<Model> CreateFromObject(const std::string& text, bool smoothing = false);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize_;

private:// �ÓI�����o�֐�
	//�}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	// �}�e���A���o�^
	void AddMaterial(Material* material);
	// �f�X�N���v�^�q�[�v�̏�����
	void CreateDescriptorHeap();
	// �e�N�X�`���ǂݍ���
	void LoadTexture();

public: // �����o�֐�
	//������
	void Initialize(const std::string& text, bool smoothing);
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�A���t�@�l��ݒ�
	void SetAlpha(float alpha);
	// ���b�V���R���e�i���擾
	inline const std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return meshes_; }

private: // �����o�ϐ�
	// ���O
	std::string name_;
	// ���b�V���R���e�i
	std::vector<std::unique_ptr<Mesh>> meshes_;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, std::unique_ptr<Material>> materials_;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial_ = nullptr;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap_;
};