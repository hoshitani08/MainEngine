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
	skydome_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("skydome"));
	ground_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("ground"));
	ground_->SetPosition({ -11,0,0 });

	for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxXY("map").x; i++)
	{
		for (int j = 0; j < MapChip::GetInstance()->GetMapChipMaxXY("map").y; j++)
		{
			std::unique_ptr<Block> block;

			XMFLOAT2 size = { MapChip::GetInstance()->GetMapChipMaxXY("map").x / 2, MapChip::GetInstance()->GetMapChipMaxXY("map").y / 2 };
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
}

void Stage::Finalize()
{
}

void Stage::Update()
{
	skydome_->Update();
	ground_->Update();

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
}
