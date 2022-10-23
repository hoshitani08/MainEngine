#include "FbxFactory.h"
#include "FbxLoader.h"

// 静的メンバ変数の実体
const std::string FbxFactory::baseDirectory = "Resources/";

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
}

void FbxFactory::Finalize()
{
	for (auto& a : modelData)
	{
		a.fbxModel.reset();
		a.fName.clear();
	}
}

void FbxFactory::LoadModel(std::string fName)
{
	ModelData tempData;
	tempData.fName = baseDirectory + fName + ".fbx";
	tempData.fbxModel = FbxLoader::GetInstance()->LoadModelFromFile(fName);

	modelData.push_back(std::move(tempData));
}

FbxModel* FbxFactory::GetModel(std::string fName)
{
	for (int i = 0; i < modelData.size(); i++)
	{
		if (modelData[i].fName == baseDirectory + fName + ".fbx")
		{
			return modelData[i].fbxModel.get();
		}
	}

	return nullptr;
}

FbxFactory* FbxFactory::GetInstance()
{
	static FbxFactory instance;
	return &instance;
}