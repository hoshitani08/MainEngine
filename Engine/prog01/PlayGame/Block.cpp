#include "Block.h"
#include "ObjFactory.h"
#include "BaseCalculate.h"

#include <sstream>
#include <iomanip>
#include <random>

Block::Block(int type, float posX, float posZ)
{
	Initialize(type, posX, posZ);
}

void Block::Initialize(int type, float posX, float posZ)
{
	if (type == (int)BlockType::Rock)
	{
		rockBlock_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("Rock"));
		rockBlock_->SetPosition({ posX, 0.8f, posZ });
		rockBlock_->SetRotation({ RandCalculate(0.0f,180.0f), 0,RandCalculate(0.0f,180.0f) });
		float size = RandCalculate(1.0f, 3.0f);
		rockBlock_->SetScale({ size, size, size });

		blockType_ = BlockType::Rock;
	}
	else if (type == (int)BlockType::Coral)
	{
		int numCount = RandCalculate(3.0f, 5.0f);

		for (int i = 0; i < numCount; i++)
		{
			CoralData tmp;
			tmp.coralBlock = Object3d::Create(ObjFactory::GetInstance()->GetModel("coral"));

			float size = RandCalculate(1.0f, 5.0f) / 10;
			tmp.coralBlock->SetScale({ size, size, size });

			tmp.coralBlock->SetPosition({ posX + RandCalculate(-size,size), size, posZ + RandCalculate(-size,size) });
			tmp.coralBlock->SetRotation({ 0, RandCalculate(0.0f,180.0f),0 });

			tmp.bubbleParticle = std::make_unique<ObjParticle>();
			tmp.bubbleEmitter = std::make_unique<ParticleEmitter>(tmp.bubbleParticle.get());
			float scale = 0.1f;
			tmp.bubbleEmitter->SetCenter(size);
			tmp.bubbleEmitter->SetObjStartScale({ scale, scale, scale });
			tmp.bubbleEmitter->SetObjEndScale({ scale, scale, scale });
			tmp.bubbleEmitter->SetStartColor({ 1,1,1,0.5f });
			tmp.bubbleEmitter->SetEndColor({ 1,1,1,0.2f });

			coralBlock_.push_back(std::move(tmp));
		}

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
		for (auto& a : coralBlock_)
		{
			if (a.bubbleTimer <= 0)
			{
				a.bubbleTimerMax = (rand() % 60) + 180;
			}
			a.bubbleTimer++;

			if (a.bubbleTimerMax <= a.bubbleTimer)
			{
				a.bubbleEmitter->BubbleAdd(4, 600, a.coralBlock->GetPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
				a.bubbleTimer = 0;
			}
		}
	}

	if (rockBlock_ != nullptr)
	{
		rockBlock_->Update();
	}
	for (auto& a : coralBlock_)
	{
		a.coralBlock->Update();
		a.bubbleEmitter->Update();
	}
}

void Block::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (rockBlock_ != nullptr)
	{
		rockBlock_->Draw(cmdList);
	}
	for (auto& a : coralBlock_)
	{
		a.coralBlock->Draw(cmdList);
		a.bubbleEmitter->Draw(cmdList);
	}
}