#include "FbxFactory.h"
#include "FbxLoader.h"

// 静的メンバ変数の実体
const std::string FbxFactory::BASE_DIRECTORY = "Resources/";

void FbxFactory::Initialize()
{
	LoadModel("player");
	// ダメージを受けた
	LoadModel("damage");
	// 移動
	LoadModel("move");
	// 攻撃
	LoadModel("attack");
	// 死んだ
	LoadModel("death");
	// 停止
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