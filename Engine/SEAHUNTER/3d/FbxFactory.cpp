#include "FbxFactory.h"
#include "FbxLoader.h"

// �ÓI�����o�ϐ��̎���
const std::string FbxFactory::BASE_DIRECTORY = "Resources/";

void FbxFactory::Initialize()
{
	LoadModel("player");
	// �_���[�W���󂯂�
	LoadModel("damage");
	// �ړ�
	LoadModel("move");
	// �U��
	LoadModel("attack");
	// ����
	LoadModel("death");
	// ��~
	LoadModel("halt");
	//
	LoadModel("dash");
	//
	LoadModel("dodge");
}

void FbxFactory::Finalize()
{
	for (auto& a : modelData_)
	{
		a.fbxModel.reset();
		a.fName.clear();
	}
}

void FbxFactory::LoadModel(std::string fName)
{
	ModelData tempData;
	tempData.fName = BASE_DIRECTORY + fName + ".fbx";
	tempData.fbxModel = FbxLoader::GetInstance()->LoadModelFromFile(fName);

	modelData_.push_back(std::move(tempData));
}

FbxModel* FbxFactory::GetModel(std::string fName)
{
	for (int i = 0; i < modelData_.size(); i++)
	{
		if (modelData_[i].fName == BASE_DIRECTORY + fName + ".fbx")
		{
			return modelData_[i].fbxModel.get();
		}
	}

	return nullptr;
}

FbxFactory* FbxFactory::GetInstance()
{
	static FbxFactory instance;
	return &instance;
}