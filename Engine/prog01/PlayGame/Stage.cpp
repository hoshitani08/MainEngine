#include "Stage.h"
#include "ObjFactory.h"
#include "MapChip.h"

Stage::Stage()
{
	MapChip::GetInstance()->CsvLoad(26, 26, "map");
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
			XMFLOAT2 count = { 5.5f, 6.0f };

			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 1)
			{
				block = std::make_unique<Block>(0, (i - size.x) * count.x, (j - size.y) * count.y);

				block_.push_back(std::move(block));
			}
			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 2)
			{
				block = std::make_unique<Block>(1, (i - size.x) * count.x, (j - size.y) * count.y);

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
