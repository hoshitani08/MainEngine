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
	nucleus_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
	nucleus_->SetPosition({ 0,10,50 });

	for (int i = 0; i < body_.size(); i++)
	{
		body_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			body_[i]->SetParent(nucleus_.get());
		}
		else
		{
			body_[i]->SetPosition({ -1.5f,0,0 });
			body_[i]->SetParent(body_[i - 1].get());
		}
	}

	for (int i = 0; i < rightArm_.size(); i++)
	{
		rightArm_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightArm_[i]->SetPosition({ -3,0,-1 });
			rightArm_[i]->SetParent(body_[0].get());
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
			leftArm_[i]->SetParent(body_[0].get());
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
			tail_[i]->SetParent(body_[4].get());
		}
		else
		{
			tail_[i]->SetPosition({ -1.5f,0,0 });
			tail_[i]->SetParent(tail_[i - 1].get());
		}
	}
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	if (hp_ >= 1)
	{
		//Move();
		Animation(0);
		//AngleAdjustment();
	}
	else
	{
		isDead_ = true;
	}




	//更新
	nucleus_->Update();
	for (int i = 0; i < body_.size(); i++)
	{
		body_[i]->Update();
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
		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->Draw();
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
	Animation(0);
	switch (phase_)
	{
	case Phase::Approach:
		AngleAdjustment();
		ApproachMove(0.4f);
		Hit(0);
		max = 30;
		break;
	case Phase::Stop:
		max = 10;
		break;
	case Phase::Attack:
		Animation(1);
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
			Hit(10);
			max = 60;
			break;
		case AttackType::Ordinary:
			AngleAdjustment();
			AttackMove(0.8f);
			Hit(20);
			max = 60;
			break;
		case AttackType::Strong:
			AngleAdjustment();
			AttackMove(1.0f);
			Hit(30);
			max = 60;
			break;
		default:
			break;
		}
		
		break;
	case Phase::Leave:
		AngleAdjustment();
		AttackMove(-1.0f);
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
		hitFlag_ = false;
	}

	/*DebugText::GetInstance()->VariablePrint(0, 0, "monster_.x", monster_->GetPosition().x, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 16, "monster_.y", monster_->GetPosition().y, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 32, "monster_.z", monster_->GetPosition().z, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 48, "count", count, 1.0f);*/
	
}

void Monster::AngleAdjustment()
{
	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };
	XMFLOAT3 enemyRot = nucleus_->GetRotation();

	enemyRot.y = -atan2(vector.z - 0, vector.x - 0) * (ANGLE / PI);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(result.y - 0, result.x - 0) * (ANGLE / PI);

	nucleus_->SetRotation(enemyRot);
}

void Monster::Hit(float damage)
{
	XMFLOAT3 enemyPos = nucleus_->GetPosition();
	XMFLOAT3 playerPos = hunter_->GetPosition();
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { enemyPos.x, enemyPos.y, enemyPos.z, 1 };
	pSphere.center = { playerPos.x, playerPos.y, playerPos.z, 1 };

	if (Collision::CheckSphere2Sphere(eSphere, pSphere))
	{
		if (hunter_->GetInvincibleTimer() >= 60)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamagePercent(damage);
		}

		hitFlag_ = true;
	}
}

void Monster::AttackMove(float speed)
{
	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };

	if (!hitFlag_)
	{
		saveVector_ = vector;
	}

	float v = sqrtf((saveVector_.x * saveVector_.x) + (saveVector_.y * saveVector_.y) + (saveVector_.z * saveVector_.z));
	saveVector_ = { (saveVector_.x / v) * speed, (saveVector_.y / v) * speed, (saveVector_.z / v) * speed };

	pos.x += saveVector_.x;
	pos.y += saveVector_.y;
	pos.z += saveVector_.z;

	nucleus_->SetPosition(pos);
}

void Monster::ApproachMove(float speed)
{
	if (!hitFlag_)
	{
		XMFLOAT3 pos = nucleus_->GetPosition();
		XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };
		float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

		pos.x += vector.x;
		pos.y += vector.y;
		pos.z += vector.z;

		nucleus_->SetPosition(pos);
	}
	else
	{
		moveTimer_ = 999;
	}
}

void Monster::Animation(int type)
{
	//基本
	if (type == 0)
	{
		XMFLOAT3 rot = rightArm_[waveCount]->GetRotation();
		XMFLOAT3 rot2 = leftArm_[waveCount]->GetRotation();
		// 幅の制限
		float restrictionAngle = 15;

		rot.z  += cosf(PI * 2 / 90 * waveTimer);
		rot2.z -= cosf(PI * 2 / 90 * waveTimer);

		rightArm_[waveCount]->SetRotation(rot);
		leftArm_[waveCount]->SetRotation(rot2);


		for (int i = 0; i < tail_.size(); i++)
		{
			XMFLOAT3 tailRot = tail_[i]->GetRotation();

			tailRot.y += cosf(PI * 2 / 90 * waveTimer);

			tail_[i]->SetRotation(tailRot);
		}

		waveTimer++;
	}
	//突進
	else if (type == 1)
	{
		XMFLOAT3 rot = body_[0]->GetRotation();

		rot.x += MAX_ANGLE;

		body_[0]->SetRotation(rot);
	}
}
