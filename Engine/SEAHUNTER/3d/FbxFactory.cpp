#include "FbxFactory.h"
#include "FbxLoader.h"

// Ã“Iƒƒ“ƒo•Ï”‚ÌŽÀ‘Ì
const std::string FbxFactory::BASE_DIRECTORY = "Resources/";

void FbxFactory::Initialize()
{
	// ƒ_ƒ[ƒW‚ðŽó‚¯‚½
	LoadModel("damage");
	// ˆÚ“®
	LoadModel("move");
	// Ž€‚ñ‚¾
	LoadModel("death");
	// ’âŽ~
	LoadModel("halt");
	// ƒ_ƒbƒVƒ…
	LoadModel("dash");
	// ‰ñ”ð
	LoadModel("dodge");
	// UŒ‚
	LoadModel("combo1");
	// UŒ‚
	LoadModel("combo2");
	// UŒ‚
	LoadModel("combo3");
}

void FbxFactory::Finalize()
{
	for (auto& a : modelData_)
	{
		a.fbxModel.reset();
		a.fName.clear();
	}
}

void FbxFactory::LoadModel(const std::string& fName)
{
	ModelData tempData;
	tempData.fName = BASE_DIRECTORY + fName + ".fbx";
	tempData.fbxModel = FbxLoader::GetInstance()->LoadModelFromFile(fName);

	modelData_.push_back(std::move(tempData));
}

FbxModel* FbxFactory::GetModel(const std::string& fName)
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