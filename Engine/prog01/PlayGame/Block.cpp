#include "Block.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"

Block::Block(int type)
{
	Initialize(type, {});
}

void Block::Initialize(int type, XMFLOAT3 position)
{
	if (type == (int)BlockType::Rock)
	{
		block_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		block_->SetPosition(position);
	}
	else if (type == (int)BlockType::Coral)
	{
		block_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("coral"));
		block_->SetPosition(position);

		bubbleParticle_ = std::make_unique<ObjParticle>();
		bubbleEmitter_ = std::make_unique<ParticleEmitter>(bubbleParticle_.get());
		bubbleEmitter_->SetCenter(1);
		bubbleEmitter_->SetObjScale({ 0.5f, 0.5f, 0.5f });
		bubbleEmitter_->SetStartColor({ 1,0,0,1 });
		bubbleEmitter_->SetEndColor({ 1,0,0,0.2f });
	}
}

void Block::Finalize()
{
}

void Block::Update()
{
	block_->Update();
	bubbleEmitter_->Update();
}

void Block::Draw(ID3D12GraphicsCommandList* cmdList)
{
	block_->Draw(cmdList);
	bubbleEmitter_->Draw(cmdList);
}