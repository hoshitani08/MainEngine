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

	for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxXY("map").x; i++)
	{
		for (int j = 0; j < MapChip::GetInstance()->GetMapChipMaxXY("map").y; j++)
		{
			std::unique_ptr<Block> block;
			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 1)
			{
				block = std::make_unique<Block>(0, (i - 13) * 5, (j - 13) * 5);

				block_.push_back(std::move(block));
			}
			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 2)
			{
				block = std::make_unique<Block>(1, (i - 13) * 5, (j - 13) * 5);

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
