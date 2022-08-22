#include "Monster.h"
#include "FbxFactory.h"
#include "Collision.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "DebugText.h"

#include <math.h>

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
	if (hp_ >= 1)
	{
		Move();
	}
	else
	{
		isDead_ = true;
	}

	monster_->Update();
}

void Monster::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	if (hp_ >= 1)
	{
		monster_->Draw(cmdList);
	}
}

void Monster::Move()
{
	XMFLOAT3 enemyPos = monster_->GetPosition();
	XMFLOAT3 playerPos = hunter_->GetPosition();
	XMFLOAT3 vector = { playerPos.x - enemyPos.x, playerPos.y - enemyPos.y, playerPos.z - enemyPos.z };
	float v = 0.0f;
	Sphere eSphere;
	Sphere pSphere;

	switch (phase_)
	{
	case Phase::Approach:
		v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector = { (vector.x / v) * 0.5f, (vector.y / v) * 0.5f, (vector.z / v) * 0.5f };

		enemyPos.x += vector.x;
		enemyPos.y += vector.y;
		enemyPos.z += vector.z;

		monster_->SetPosition(enemyPos);
		max = 30;
		break;
	case Phase::Stop:
		max = 10;
		break;
	case Phase::Attack:
		if (moveTimer_ <= 0)
		{
			count = rand() % 4;
			if (count == 0)
			{
				attackType_ = AttackType::Weak;
			}
			else if (count == 1)
			{
				attackType_ = AttackType::Ordinary;
			}
			else if (count == 2)
			{
				attackType_ = AttackType::Strong;
			}
		}
		switch (attackType_)
		{
		case AttackType::Weak:
			v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
			vector = { (vector.x / v) * 1.0f, (vector.y / v) * 1.0f, (vector.z / v) * 1.0f };

			enemyPos.x += vector.x;
			enemyPos.y += vector.y;
			enemyPos.z += vector.z;

			monster_->SetPosition(enemyPos);


			eSphere.center = { enemyPos.x, enemyPos.y, enemyPos.z, 1 };

			pSphere.center = { playerPos.x, playerPos.y, playerPos.z, 1 };

			if (Collision::CheckSphere2Sphere(eSphere, pSphere) && hunter_->GetInvincibleTimer() >= 60)
			{
				hunter_->SetDamageFlag(true);
				hunter_->SetDamagePercent(10);
			}

			max = 60;
			break;
		case AttackType::Ordinary:
			v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
			vector = { (vector.x / v) * 0.8f, (vector.y / v) * 0.8f, (vector.z / v) * 0.8f };

			enemyPos.x += vector.x;
			enemyPos.y += vector.y;
			enemyPos.z += vector.z;

			monster_->SetPosition(enemyPos);


			eSphere.center = { enemyPos.x, enemyPos.y, enemyPos.z, 1 };

			pSphere.center = { playerPos.x, playerPos.y, playerPos.z, 1 };

			if (Collision::CheckSphere2Sphere(eSphere, pSphere) && hunter_->GetInvincibleTimer() >= 60)
			{
				hunter_->SetDamageFlag(true);
				hunter_->SetDamagePercent(30);
			}

			max = 60;
			break;
		case AttackType::Strong:
			v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
			vector = { (vector.x / v) * 1.0f, (vector.y / v) * 1.0f, (vector.z / v) * 1.0f };

			enemyPos.x += vector.x;
			enemyPos.y += vector.y;
			enemyPos.z += vector.z;

			monster_->SetPosition(enemyPos);


			eSphere.center = { enemyPos.x, enemyPos.y, enemyPos.z, 1 };

			pSphere.center = { playerPos.x, playerPos.y, playerPos.z, 1 };

			if (Collision::CheckSphere2Sphere(eSphere, pSphere) && hunter_->GetInvincibleTimer() >= 60)
			{
				hunter_->SetDamageFlag(true);
				hunter_->SetDamagePercent(50);
			}

			max = 60;
			break;
		default:
			break;
		}
		
		break;
	case Phase::Leave:
		v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector = { (vector.x / v) * -0.8f, (vector.y / v) * -0.8f, (vector.z / v) * -0.8f };

		enemyPos.x += vector.x;
		enemyPos.y += vector.y;
		enemyPos.z += vector.z;

		monster_->SetPosition(enemyPos);
		max = 20;
		break;
	default:
		break;
	}

	moveTimer_++;
	
	if (moveTimer_ >= max)
	{
		count = rand() % 4;
		if (count == 0)
		{
			phase_ = Phase::Approach;
		}
		else if (count == 1)
		{
			phase_ = Phase::Stop;
		}
		else if (count == 2)
		{
			phase_ = Phase::Attack;
		}
		else if (count == 3)
		{
			phase_ = Phase::Leave;
		}
		
		moveTimer_ = 0;
	}

	/*DebugText::GetInstance()->VariablePrint(0, 0, "monster_.x", monster_->GetPosition().x, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 16, "monster_.y", monster_->GetPosition().y, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 32, "monster_.z", monster_->GetPosition().z, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 48, "count", count, 1.0f);*/
	
}
