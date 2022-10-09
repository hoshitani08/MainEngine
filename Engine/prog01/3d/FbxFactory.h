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
		std::string fName;
		std::unique_ptr<FbxModel> fbxModel;
	};

public:
	// ���[�g�p�X
	static const std::string baseDirectory;

private:
	FbxFactory() = default;
	~FbxFactory() = default;

public:
	FbxFactory(const FbxFactory & fbxFactory) = delete;
	FbxFactory& operator=(const FbxFactory & fbxFactory) = delete;

	// ������
	void Initialize();
	// �I��
	void Finalize();
	// ���f���̃��[�h
	void LoadModel(std::string fName);
	// ���f���̎擾
	FbxModel* GetModel(std::string fName);
	// �C���X�^���X����
	static FbxFactory* GetInstance();

private:
	std::vector<ModelData> modelData;
};