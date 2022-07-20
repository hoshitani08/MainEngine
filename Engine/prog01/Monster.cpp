#include "Monster.h"
#include "FbxFactory.h"

#include "DirectXCommon.h"
#include "Input.h"

std::unique_ptr<Monster> Monster::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Monster* monster = new Monster();
	if (monster == nullptr)
	{
		return nullptr;
	}
	monster->Initialize();

	return std::unique_ptr<Monster>(monster);
}

void Monster::Initialize()
{
	float size = 0.006f;
	monster_ = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("a"), L"BasicFBX");
	monster_->SetScale({ size, size, size });
	monster_->SetPosition({ 0, 10, 50 });
	//fbxObject3d->PlayAnimation(2);
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	Move();
	monster_->Update();
}

void Monster::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	monster_->Draw(cmdList);
}

void Monster::Move()
{
	switch (phase_)
	{
	case Phase::Approach:
		break;
	case Phase::Stop:
		break;
	case Phase::Attack:
		break;
	case Phase::Leave:
		break;
	default:
		break;
	}
}

void Monster::Attack()
{
}
