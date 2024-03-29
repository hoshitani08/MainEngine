#include "ObjFactory.h"

// 静的メンバ変数の実体
const std::string ObjFactory::BASE_DIRECTORY = "Resources/";

void ObjFactory::Initialize()
{
	LoadModel("watersurface");
	LoadModel("ground");
	LoadModel("title");
	LoadModel("quit");
	LoadModel("start");
	LoadModel("clear");
	LoadModel("continue");
	LoadModel("gameover");
	LoadModel("sphere");
	LoadModel("katana");
	LoadModel("coral");
	LoadModel("Rock");
	LoadModel("Rock2");
	LoadModel("cube");
	LoadModel("bubble", true);
	LoadModel("blood", true);
	LoadModel("heal");
	LoadModel("itemEffect");
	LoadModel("wall");
	LoadModel("post");
}

void ObjFactory::Finalize()
{
	for (auto& a : modelData_)
	{
		a.model.reset();
		a.fName.clear();
	}
}

void ObjFactory::LoadModel(const std::string& fName, bool smoothing)
{
	ModelData tempData;
	tempData.fName = BASE_DIRECTORY + fName + ".obj";
	tempData.model = Model::CreateFromObject(fName, smoothing);

	modelData_.push_back(std::move(tempData));
}

Model* ObjFactory::GetModel(const std::string& fName)
{
	for (int i = 0; i < modelData_.size(); i++)
	{
		if (modelData_[i].fName == BASE_DIRECTORY + fName + ".obj")
		{
			return modelData_[i].model.get();
		}
	}

	return nullptr;
}

ObjFactory* ObjFactory::GetInstance()
{
	static ObjFactory instance;
	return &instance;
}