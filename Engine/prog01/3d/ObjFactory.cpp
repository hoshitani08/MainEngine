#include "ObjFactory.h"

// ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃ
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
	LoadModel("cube");
}

void ObjFactory::Finalize()
{
	for (auto& a : modelData_)
	{
		a.model.reset();
		a.fName.clear();
	}
}

void ObjFactory::LoadModel(std::string fName, bool smoothing)
{
	ModelData tempData;
	tempData.fName = BASE_DIRECTORY + fName + ".obj";
	tempData.model = Model::CreateFromObject(fName, smoothing);

	modelData_.push_back(std::move(tempData));
}

Model* ObjFactory::GetModel(std::string fName)
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