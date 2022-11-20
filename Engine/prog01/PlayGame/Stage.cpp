#include "Stage.h"
#include "ObjFactory.h"
#include "MapChip.h"

Stage::Stage()
{
	Initialize();
}

void Stage::Initialize()
{
	skydome_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("watersurface"));
	skydome_->SetPosition({ 0,100,0 });
	skydome_->SetScale({ 2,2,2 });
	ground_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("ground"));
	ground_->SetScale({ 2,2,2 });
	ground_->SetPosition({ -11,0,0 });

	for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxX("map"); i++)
	{
		for (int j = 0; j < MapChip::GetInstance()->GetMapChipMaxY("map"); j++)
		{
			std::unique_ptr<Block> block;

			XMFLOAT2 size = { (float)MapChip::GetInstance()->GetMapChipMaxX("map") / 2, (float)MapChip::GetInstance()->GetMapChipMaxY("map") / 2 };
			float count = 6.0f;
			XMFLOAT3 pos = { (i - size.x) * count, 0, (j - size.y) * count };

			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 1)
			{
				block = std::make_unique<Block>(0, pos);

				block_.push_back(std::move(block));
			}
			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 2)
			{
				block = std::make_unique<Block>(1, pos);

				block_.push_back(std::move(block));
			}
		}
	}

	field_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("cube"));
	float size = 50.0f;
	field_->SetScale({ size ,30 ,size });
	field_->SetPrimitiveType(ShaderManager::Type::Line);
}

void Stage::Finalize()
{
}

void Stage::Update()
{
	skydome_->Update();
	ground_->Update();
	field_->Update();

	for (auto& a : block_)
	{
		a->Update();
	}
}

void Stage::Draw(ID3D12GraphicsCommandList* cmdList)
{
	skydome_->Draw(cmdList);
	ground_->Draw(cmdList);
	for (auto& a : block_)
	{
		a->Draw(cmdList);
	}
	field_->Draw(cmdList);
}
