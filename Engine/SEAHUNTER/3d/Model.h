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
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// OBJ�t�@�C�����烁�b�V������
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <param name="modelname">�G�b�W�������t���O</param>
	/// <returns>�������ꂽ���f��</returns>
	static std::unique_ptr<Model> CreateFromObject(const std::string& text, bool smoothing = false);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize_;

private:// �ÓI�����o�֐�
	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�ǂݍ��݃f�B���N�g���p�X</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	/// <param name="material">�}�e���A��</param>
	void AddMaterial(Material* material);
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	void CreateDescriptorHeap();
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="text">���f����</param>
	/// <param name="smoothing">�G�b�W�������t���O</param>
	void Initialize(const std::string& text, bool smoothing);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �A���t�@�l��ݒ�
	/// </summary>
	/// <param name="alpha">�A���t�@�l</param>
	void SetAlpha(float alpha);
	/// <summary>
	/// ���b�V���R���e�i���擾
	/// </summary>
	/// <returns>���b�V���R���e�i</returns>
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