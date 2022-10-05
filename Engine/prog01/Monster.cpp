#include "Monster.h"
#include "FbxFactory.h"
#include "Collision.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "DebugText.h"
#include "ObjFactory.h"

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
	for (int i = 0; i < nucleus_.size(); i++)
	{
		nucleus_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));

		if (i != 0)
		{
			nucleus_[i]->SetPosition({ -1.5f,0,0 });
			nucleus_[i]->SetParent(nucleus_[i - 1].get());
		}
	}

	for (int i = 0; i < rightArm_.size(); i++)
	{
		rightArm_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightArm_[i]->SetPosition({ -3,0,-1 });
			rightArm_[i]->SetParent(nucleus_[0].get());
		}
		else
		{
			rightArm_[i]->SetPosition({ -1.5f,0,-1 });
			rightArm_[i]->SetParent(rightArm_[i - 1].get());
		}
	}

	for (int i = 0; i < leftArm_.size(); i++)
	{
		leftArm_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftArm_[i]->SetPosition({ -3,0,1 });
			leftArm_[i]->SetParent(nucleus_[0].get());
		}
		else
		{
			leftArm_[i]->SetPosition({ -1.5f,0,1 });
			leftArm_[i]->SetParent(leftArm_[i - 1].get());
		}
	}

	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			tail_[i]->SetPosition({ -1.5f,0,0 });
			tail_[i]->SetParent(nucleus_[4].get());
		}
		else
		{
			tail_[i]->SetPosition({ -1.5f,0,0 });
			tail_[i]->SetParent(tail_[i - 1].get());
		}
	}

	nucleus_[0]->SetPosition({ 0,10,50 });
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




	//更新
	for (int i = 0; i < nucleus_.size(); i++)
	{
		nucleus_[i]->Update();
	}
	for (int i = 0; i < rightArm_.size(); i++)
	{
		rightArm_[i]->Update();
	}
	for (int i = 0; i < leftArm_.size(); i++)
	{
		leftArm_[i]->Update();
	}
	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i]->Update();
	}
}

void Monster::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	Object3d::PreDraw(cmdList);
	if (hp_ >= 1)
	{
		for (int i = 0; i < nucleus_.size(); i++)
		{
			nucleus_[i]->Draw();
		}
		for (int i = 0; i < rightArm_.size(); i++)
		{
			rightArm_[i]->Draw();
		}
		for (int i = 0; i < leftArm_.size(); i++)
		{
			leftArm_[i]->Draw();
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			tail_[i]->Draw();
		}
	}
	Object3d::PostDraw();
	
}

void Monster::Move()
{
	switch (phase_)
	{
	case Phase::Approach:
		AngleAdjustment();
		AttackMove(0.5f);
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
			AngleAdjustment();
			AttackMove(0.5f);
			Hit();
			max = 60;
			break;
		case AttackType::Ordinary:
			AngleAdjustment();
			AttackMove(0.8f);
			Hit();
			max = 60;
			break;
		case AttackType::Strong:
			AngleAdjustment();
			AttackMove(1.0f);
			Hit();
			max = 60;
			break;
		default:
			break;
		}
		
		break;
	case Phase::Leave:
		AngleAdjustment();
		AttackMove(-0.8f);
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

void Monster::AngleAdjustment()
{

	XMFLOAT3 enemyPos = nucleus_[0]->GetPosition();
	XMFLOAT3 playerPos = hunter_->GetPosition();
	XMFLOAT3 vector = { playerPos.x - enemyPos.x, playerPos.y - enemyPos.y, playerPos.z - enemyPos.z };
	XMFLOAT3 enemyRot = nucleus_[0]->GetRotation();

	XMFLOAT3 radian = { atan2(vector.z - 0, vector.x - 0), atan2(vector.y - 0, vector.x - 0), atan2(vector.y - 0, vector.z - 0) };
	enemyRot.y = -radian.x * (180.0f / 3.14159265359f);

	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(result.y - 0, result.x - 0) * (180.0f / 3.14159265359f);

	nucleus_[0]->SetRotation(enemyRot);
}

void Monster::Hit()
{
	XMFLOAT3 enemyPos = nucleus_[0]->GetPosition();
	XMFLOAT3 playerPos = hunter_->GetPosition();
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { enemyPos.x, enemyPos.y, enemyPos.z, 1 };
	pSphere.center = { playerPos.x, playerPos.y, playerPos.z, 1 };

	if (Collision::CheckSphere2Sphere(eSphere, pSphere) && hunter_->GetInvincibleTimer() >= 60)
	{
		hunter_->SetDamageFlag(true);
		hunter_->SetDamagePercent(10);
	}
}

void Monster::AttackMove(float speed)
{
	XMFLOAT3 pos = nucleus_[0]->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_[0]->GetPosition().x, hunter_->GetPosition().y - nucleus_[0]->GetPosition().y, hunter_->GetPosition().z - nucleus_[0]->GetPosition().z };

	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	nucleus_[0]->SetPosition(pos);
}
