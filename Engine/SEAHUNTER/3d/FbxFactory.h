#pragma once

#include <string>
#include <vector>
#include <memory>

#include "FbxModel.h"
#include "FbxLoader.h"

class FbxFactory final
{
public: // �T�u�N���X
	struct ModelData
	{
		std::string fName; // �t�@�C����
		std::unique_ptr<FbxModel> fbxModel; // FBX���f��
	};

public:
	// ���[�g�p�X
	static const std::string BASE_DIRECTORY;

private:
	FbxFactory() = default;
	~FbxFactory() = default;

public:
	FbxFactory(const FbxFactory & fbxFactory) = delete;
	FbxFactory& operator=(const FbxFactory & fbxFactory) = delete;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	void LoadModel(std::string fName);
	/// <summary>
	/// ���f���̎擾
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	/// <returns>�����������f��</returns>
	FbxModel* GetModel(std::string fName);
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static FbxFactory* GetInstance();

private:
	// ���f���f�[�^
	std::vector<ModelData> modelData_;
};