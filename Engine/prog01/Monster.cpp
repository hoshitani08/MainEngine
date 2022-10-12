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




	//更新
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
	// コマンドリストの取得
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
	switch (phase_)
	{
	case Phase::Approach:
		Animation(AnimationType::Move);
		AngleAdjustment();
		Move(0.8f);
		break;
	case Phase::Stop:
		Animation(AnimationType::Stop);
		break;
	case Phase::Attack:
		Animation(AnimationType::Assault);
		if (!saveFlag_)
		{
			AngleAdjustment();
		}
		Move(1.0f);
		break;
	case Phase::Leave:
		Animation(AnimationType::Move);
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

void Monster::DamageHit(XMFLOAT3 partsPosition, float enemyRange, float playerRange, float damage)
{
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { partsPosition.x, partsPosition.y, partsPosition.z, 1 };
	eSphere.radius = enemyRange;
	pSphere.center = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z, 1 };
	pSphere.radius = playerRange;

	if (Collision::CheckSphere2Sphere(eSphere, pSphere) && !hitFlag_)
	{
		if (hunter_->GetInvincibleTimer() >= 60)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(damage);
			hitFlag_ = true;
		}
	}
}

void Monster::RangeHit(XMFLOAT3 partsPosition, float enemyRange, float playerRange)
{
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { partsPosition.x, partsPosition.y, partsPosition.z, 1 };
	eSphere.radius = enemyRange;
	pSphere.center = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z, 1 };
	pSphere.radius = playerRange;

	if (Collision::CheckSphere2Sphere(eSphere, pSphere))
	{
		saveFlag_ = true;
	}
}

bool Monster::Hit(XMFLOAT3 partsPosition, float enemyRange, float playerRange)
{
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { partsPosition.x, partsPosition.y, partsPosition.z, 1 };
	eSphere.radius = enemyRange;
	pSphere.center = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z, 1 };
	pSphere.radius = playerRange;

	if (Collision::CheckSphere2Sphere(eSphere, pSphere))
	{
		return true;
	}
	return false;
}

void Monster::Move(float speed)
{
	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	switch (phase_)
	{
	case Phase::Approach:
		if (!saveFlag_)
		{
			saveVector_ = vector;
		}
		RangeHit(nucleus_->GetPosition(), 1.0f, 40);
		break;
	case Phase::Stop:
		break;
	case Phase::Attack:
		if (!saveFlag_)
		{
			saveVector_ = vector;
		}
		RangeHit(nucleus_->GetPosition(), 1.0f, 20);
		DamageHit(nucleus_->GetPosition(), 1.0f, 1.0f, 10);
		break;
	case Phase::Leave:
		if (!saveFlag_)
		{
			saveVector_ = vector;
			saveFlag_ = true;
		}
		break;
	case Phase::CoolTime:
		break;
	default:
		break;
	}

	pos.x += saveVector_.x;
	pos.y += saveVector_.y;
	pos.z += saveVector_.z;

	nucleus_->SetPosition(pos);
}

void Monster::Animation(AnimationType type)
{
	// 初期姿勢
	if (type == AnimationType::Stop)
	{
		XMFLOAT3 rot = {};

		for (int i = 0; i < body_.size(); i++)
		{
			if (rot.x == body_[i]->GetRotation().x,
				rot.y == body_[i]->GetRotation().y,
				rot.z == body_[i]->GetRotation().z)
			{
				continue;
			}
			body_[i]->SetRotation(rot);
		}
		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			if (rot.x == rightForeFoot_[i]->GetRotation().x,
				rot.y == rightForeFoot_[i]->GetRotation().y,
				rot.z == rightForeFoot_[i]->GetRotation().z)
			{
				continue;
			}
			rightForeFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			if (rot.x == leftForeFoot_[i]->GetRotation().x,
				rot.y == leftForeFoot_[i]->GetRotation().y,
				rot.z == leftForeFoot_[i]->GetRotation().z)
			{
				continue;
			}
			leftForeFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			if (rot.x == rightHindFoot_[i]->GetRotation().x,
				rot.y == rightHindFoot_[i]->GetRotation().y,
				rot.z == rightHindFoot_[i]->GetRotation().z)
			{
				continue;
			}
			rightHindFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			if (rot.x == leftHindFoot_[i]->GetRotation().x,
				rot.y == leftHindFoot_[i]->GetRotation().y,
				rot.z == leftHindFoot_[i]->GetRotation().z)
			{
				continue;
			}
			leftHindFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			if (rot.x == tail_[i]->GetRotation().x,
				rot.y == tail_[i]->GetRotation().y,
				rot.z == tail_[i]->GetRotation().z)
			{
				continue;
			}
			tail_[i]->SetRotation(rot);
		}
	}
	// 歩行
	else if (type == AnimationType::Move)
	{
		XMFLOAT3 rot = rightForeFoot_[0]->GetRotation();
		XMFLOAT3 rot2 = leftForeFoot_[0]->GetRotation();
		// 幅の制限
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

		waveTimer_ += 4;
	}
	//突進
	else if (type == AnimationType::Assault)
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
	switch (phase_)
	{
	case Phase::Approach:
		// タイマーの最大値
		maxTime_ = 30;
		if (saveFlag_)
		{
			// タイマーの加算
			moveTimer_++;

			if (Hit(nucleus_->GetPosition()))
			{
				moveTimer_ = 999;
			}
		}

		if (moveTimer_ >= maxTime_)
		{
			actEndFlag_ = true;
		}
		break;
	case Phase::Stop:
		// タイマーの最大値
		maxTime_ = 30;
		// タイマーの加算
		moveTimer_++;
		if (moveTimer_ >= maxTime_)
		{
			actEndFlag_ = true;
		}
		break;
	case Phase::Attack:
		// タイマーの最大値
		maxTime_ = 60;
		if (saveFlag_)
		{
			// タイマーの加算
			moveTimer_++;
		}
		if (body_[0]->GetRotation().x <= 0 && moveTimer_ >= maxTime_)
		{
			actEndFlag_ = true;
		}
		break;
	case Phase::Leave:
		// タイマーの最大値
		maxTime_ = 30;
		// タイマーの加算
		moveTimer_++;
		if (moveTimer_ >= maxTime_)
		{
			actEndFlag_ = true;
		}

		break;
	case Phase::CoolTime:
		Animation(AnimationType::Stop);
		moveTimer_ = 0;
		saveFlag_ = false;
		hitFlag_ = false;
		break;
	default:
		break;
	}

	if (actEndFlag_ && coolTimer < 30)
	{
		coolTimer++;
		phase_ = Phase::CoolTime;
	}
	else if(coolTimer >= 30)
	{
		actCount_ = rand() % 101;
		if (actCount_ >= 0 && actCount_ <= 49)
		{
			phase_ = Phase::Approach;
		}
		else if (actCount_ >= 50 && actCount_ <= 59)
		{
			phase_ = Phase::Stop;
		}
		else if (actCount_ >= 60 && actCount_ <= 89)
		{
			phase_ = Phase::Attack;
		}
		else if (actCount_ >= 90 && actCount_ <= 100)
		{
			phase_ = Phase::Leave;
		}
		
		coolTimer = 0;
		actEndFlag_ = false;
	}
}
