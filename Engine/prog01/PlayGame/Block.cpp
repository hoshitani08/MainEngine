#include "Block.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"

#include <sstream>
#include <iomanip>

Block::Block(int type, float posX, float posZ)
{
	Initialize(type, posX, posZ);
}

void Block::Initialize(int type, float posX, float posZ)
{
	if (type == (int)BlockType::Rock)
	{
		block_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		block_->SetPosition({ posX, 1.0f, posZ });

		blockType_ = BlockType::Rock;
	}
	else if (type == (int)BlockType::Coral)
	{
		block_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("coral"));

		int count = (rand() % 5) + 1;
		float size = (float)count / 10;
		block_->SetScale({ size, size, size });
		block_->SetPosition({ posX, size, posZ });

		bubbleParticle_ = std::make_unique<ObjParticle>();
		bubbleEmitter_ = std::make_unique<ParticleEmitter>(bubbleParticle_.get());
		bubbleEmitter_->SetCenter(1);
		bubbleEmitter_->SetObjScale({ size, size, size });
		bubbleEmitter_->SetStartColor({ 10,10,10,0.5f });
		bubbleEmitter_->SetEndColor({ 10,10,10,0.2f });

		blockType_ = BlockType::Coral;
	}
}

void Block::Finalize()
{
}

void Block::Update()
{
	if (blockType_ == BlockType::Coral)
	{
		if (bubbleTimer_ <= 0)
		{
			bubbleTimerMax_ = (rand() % 180) + 120;
		}
		bubbleTimer_++;
		
		if(bubbleTimerMax_ <= bubbleTimer_)
		{
			bubbleEmitter_->BubbleAdd(4, 120, block_->GetPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
			bubbleTimer_ = 0;
		}
	}

	block_->Update();
	bubbleEmitter_->Update();
}

void Block::Draw(ID3D12GraphicsCommandList* cmdList)
{
	block_->Draw(cmdList);
	bubbleEmitter_->Draw(cmdList);
}