#include "Block.h"
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
		rockBlock_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		rockBlock_->SetPosition({ posX, 1.0f, posZ });

		blockType_ = BlockType::Rock;
	}
	else if (type == (int)BlockType::Coral)
	{
		int numCount = (rand() % 2) + 3;

		for (int i = 0; i < numCount; i++)
		{
			XMFLOAT2 randCount = { (float)(rand() % 2) - 2, (float)(rand() % 2) - 2 };

			CoralData tmp;
			tmp.coralBlock = Object3d::Create(ObjFactory::GetInstance()->GetModel("coral"));

			int count = (rand() % 5) + 1;
			float size = (float)count / 10;
			tmp.coralBlock->SetScale({ size, size, size });
			tmp.coralBlock->SetPosition({ posX + randCount.x, size, posZ + randCount.y });

			int rot = rand() % 181 * i;
			tmp.coralBlock->SetRotation({ 0,(float)rot,0 });

			tmp.bubbleParticle = std::make_unique<ObjParticle>();
			tmp.bubbleEmitter = std::make_unique<ParticleEmitter>(tmp.bubbleParticle.get());
			tmp.bubbleEmitter->SetCenter(1);
			tmp.bubbleEmitter->SetObjScale({ size, size, size });
			tmp.bubbleEmitter->SetStartColor({ 10,10,10,0.5f });
			tmp.bubbleEmitter->SetEndColor({ 10,10,10,0.2f });

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
				a.bubbleTimerMax = (rand() % 500) + 180;
			}
			a.bubbleTimer++;

			if (a.bubbleTimerMax <= a.bubbleTimer)
			{
				a.bubbleEmitter->BubbleAdd(4, 120, a.coralBlock->GetPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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