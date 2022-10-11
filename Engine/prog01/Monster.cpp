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
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
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

		body_[i]->SetColor({ 0.4f,0.8f,0.4f,1 });
	}

	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightForeFoot_[i]->SetPosition({ -1.5f,0,-1 });
			rightForeFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			rightForeFoot_[i]->SetPosition({ 0,-1.0f,-1 });
			rightForeFoot_[i]->SetParent(rightForeFoot_[i - 1].get());
		}
		else
		{
			rightForeFoot_[i]->SetPosition({ 1,-1.0f,0 });
			rightForeFoot_[i]->SetParent(rightForeFoot_[i - 1].get());
		}
		rightForeFoot_[i]->SetColor({ 1,0.5f,0.25f,1 });
	}

	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftForeFoot_[i]->SetPosition({ -1.5f,0,1 });
			leftForeFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			leftForeFoot_[i]->SetPosition({ 0,-1.0f,1 });
			leftForeFoot_[i]->SetParent(leftForeFoot_[i - 1].get());
		}
		else
		{
			leftForeFoot_[i]->SetPosition({ 1,-1.0f,0 });
			leftForeFoot_[i]->SetParent(leftForeFoot_[i - 1].get());
		}
		leftForeFoot_[i]->SetColor({ 1,0.5f,0.25f,1 });
	}

	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightHindFoot_[i]->SetPosition({ -5.5f,0,-1 });
			rightHindFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			rightHindFoot_[i]->SetPosition({ 0,-1.0f,-1 });
			rightHindFoot_[i]->SetParent(rightHindFoot_[i - 1].get());
		}
		else
		{
			rightHindFoot_[i]->SetPosition({ -1,-1.0f,0 });
			rightHindFoot_[i]->SetParent(rightHindFoot_[i - 1].get());
		}
		rightHindFoot_[i]->SetColor({ 0,0.5f,0.5f,1 });
	}

	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftHindFoot_[i]->SetPosition({ -5.5f,0,1 });
			leftHindFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			leftHindFoot_[i]->SetPosition({ 0,-1.0f,1 });
			leftHindFoot_[i]->SetParent(leftHindFoot_[i - 1].get());
		}
		else
		{
			leftHindFoot_[i]->SetPosition({ -1,-1.0f,0 });
			leftHindFoot_[i]->SetParent(leftHindFoot_[i - 1].get());
		}

		leftHindFoot_[i]->SetColor({ 0,0.5f,0.5f,1 });
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

		tail_[i]->SetColor({ 0.5f,0.5f,0,1 });
	}
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	if (hp_ >= 1)
	{
		Activity();
		//Animation(0);
		//AngleAdjustment();
	}
	else
	{
		isDead_ = true;
	}




	//�X�V
	nucleus_->Update();
	for (int i = 0; i < body_.size(); i++)
	{
		body_[i]->Update();
	}
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i]->Update();
	}
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i]->Update();
	}
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i]->Update();
	}
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i]->Update();
	}
	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i]->Update();
	}
}

void Monster::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	Object3d::PreDraw(cmdList);
	if (hp_ >= 1)
	{
		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->Draw();
		}
		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			rightForeFoot_[i]->Draw();
		}
		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			leftForeFoot_[i]->Draw();
		}
		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			rightHindFoot_[i]->Draw();
		}
		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			leftHindFoot_[i]->Draw();
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			tail_[i]->Draw();
		}
	}
	Object3d::PostDraw();
	
}

void Monster::Activity()
{
	Animation(0);
	switch (phase_)
	{
	case Phase::Approach:
		AngleAdjustment();
		ApproachMove(0.4f);
		Hit(0);
		break;
	case Phase::Stop:
		break;
	case Phase::Attack:
		Animation(1);
		if (moveTimer_ <= 0)
		{
			actCount_ = rand() % 4;
			if (actCount_ == 0)
			{
				attackType_ = AttackType::Weak;
			}
			else if (actCount_ == 1)
			{
				attackType_ = AttackType::Ordinary;
			}
			else if (actCount_ == 2)
			{
				attackType_ = AttackType::Strong;
			}
		}
		switch (attackType_)
		{
		case AttackType::Weak:
			AngleAdjustment();
			Move(0.5f);
			Hit(10);
			break;
		case AttackType::Ordinary:
			AngleAdjustment();
			Move(0.8f);
			Hit(20);
			break;
		case AttackType::Strong:
			AngleAdjustment();
			Move(1.0f);
			Hit(30);
			break;
		default:
			break;
		}
		
		break;
	case Phase::Leave:
		AngleAdjustment();
		Move(-1.0f);
		break;
	default:
		break;
	}

	ActEnd();

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

void Monster::Move(float speed)
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
	//��{
	if (type == 0)
	{
		XMFLOAT3 rot = rightForeFoot_[0]->GetRotation();
		XMFLOAT3 rot2 = leftForeFoot_[0]->GetRotation();
		// ���̐���
		float restrictionAngle = 15;

		rot.z  += cosf(PI * 2 / 135 * waveTimer_);
		rot2.z -= cosf(PI * 2 / 135 * waveTimer_);

		rightForeFoot_[0]->SetRotation(rot);
		leftForeFoot_[0]->SetRotation(rot2);
		leftHindFoot_[0]->SetRotation(rot);
		rightHindFoot_[0]->SetRotation(rot2);


		for (int i = 0; i < tail_.size(); i++)
		{
			XMFLOAT3 tailRot = tail_[i]->GetRotation();

			tailRot.y += cosf(PI * 2 / 45 * (waveTimer_ / 2));

			tail_[i]->SetRotation(tailRot);
		}

		waveTimer_ += 2;
	}
	//�ːi
	else if (type == 1)
	{
		XMFLOAT3 rot = body_[0]->GetRotation();

		rot.x += MAX_ANGLE;

		if (rot.x >= 360)
		{
			rot.x = 0;
		}

		body_[0]->SetRotation(rot);
	}
}

void Monster::ActEnd()
{
	moveTimer_++;

	switch (phase_)
	{
	case Phase::Approach:
		maxTime_ = 30;
		break;
	case Phase::Stop:
		maxTime_ = 10;
		break;
	case Phase::Attack:
		maxTime_ = 60;
		break;
	case Phase::Leave:
		maxTime_ = 20;
		break;
	default:
		break;
	}

	if (moveTimer_ >= maxTime_)
	{
		actCount_ = rand() % 4;
		if (actCount_ == 0)
		{
			phase_ = Phase::Approach;
		}
		else if (actCount_ == 1)
		{
			phase_ = Phase::Stop;
		}
		else if (actCount_ == 2)
		{
			phase_ = Phase::Attack;
		}
		else if (actCount_ == 3)
		{
			phase_ = Phase::Leave;
		}
		moveTimer_ = 0;
		hitFlag_ = false;
	}
}
