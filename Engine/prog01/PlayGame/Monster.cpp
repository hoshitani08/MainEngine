#include "Monster.h"
#include "FbxFactory.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "DebugText.h"
#include "ObjFactory.h"
#include "ItemManager.h"
#include "Ease.h"
#include "BaseCalculate.h"

#include <math.h>

std::unique_ptr<Monster> Monster::Create(Camera* camera, Hunter* hunter)
{
	// 3Dオブジェクトのインスタンスを生成
	Monster* monster = new Monster();
	if (monster == nullptr)
	{
		return nullptr;
	}
	monster->SetHunter(hunter);
	monster->Initialize(camera);

	return std::unique_ptr<Monster>(monster);
}

void Monster::Initialize(Camera* camera)
{
	nucleus_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
	nucleus_->SetPosition({ 0,10,20 });

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
			body_[i]->SetParent(body_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 5Ui64>::size_type>(i) - 1].get());
		}

		body_[i]->SetColor({ 0.4f,0.8f,0.4f,1 });
	}

	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightForeFoot_[i]->SetPosition({ 0,0,-1 });
			rightForeFoot_[i]->SetParent(body_[1].get());
		}
		else if (i == 1)
		{
			rightForeFoot_[i]->SetPosition({ 0,-1.0f,-1 });
			rightForeFoot_[i]->SetParent(rightForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			rightForeFoot_[i]->SetPosition({ 1,-1.0f,0 });
			rightForeFoot_[i]->SetParent(rightForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		rightForeFoot_[i]->SetColor({ 1,0.5f,0.25f,1 });
	}

	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftForeFoot_[i]->SetPosition({ 0,0,1 });
			leftForeFoot_[i]->SetParent(body_[1].get());
		}
		else if (i == 1)
		{
			leftForeFoot_[i]->SetPosition({ 0,-1.0f,1 });
			leftForeFoot_[i]->SetParent(leftForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			leftForeFoot_[i]->SetPosition({ 1,-1.0f,0 });
			leftForeFoot_[i]->SetParent(leftForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
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
			rightHindFoot_[i]->SetParent(rightHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			rightHindFoot_[i]->SetPosition({ -1,-1.0f,0 });
			rightHindFoot_[i]->SetParent(rightHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
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
			leftHindFoot_[i]->SetParent(leftHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			leftHindFoot_[i]->SetPosition({ -1,-1.0f,0 });
			leftHindFoot_[i]->SetParent(leftHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
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
			tail_[i]->SetParent(tail_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 4Ui64>::size_type>(i) - 1].get());
		}

		tail_[i]->SetColor({ 0.5f,0.5f,0,1 });
	}

	bloodParticle_ = std::make_unique<ObjParticle>();
	bloodEmitter_ = std::make_unique<ParticleEmitter>(bloodParticle_.get());
	bloodEmitter_->SetCenter(1);
	float scale = 0.4f;
	bloodEmitter_->SetObjStartScale({ scale, scale, scale });
	bloodEmitter_->SetStartColor({ 1,0,0,1 });
	bloodEmitter_->SetEndColor({ 1,0,0,1 });

	bubbleParticle_ = std::make_unique<ObjParticle>();
	bubbleEmitter_ = std::make_unique<ParticleEmitter>(bubbleParticle_.get());
	scale = 0.1f;
	bubbleEmitter_->SetCenter(1);
	bubbleEmitter_->SetObjStartScale({ scale, scale, scale });
	bubbleEmitter_->SetObjEndScale({ scale, scale, scale });
	bubbleEmitter_->SetStartColor({ 1,1,1,1.0f });
	bubbleEmitter_->SetEndColor({ 1,1,1,0.0f });

	//ビヘイビアツリーの初期化
	activitySelector_.push_back(std::bind(&Monster::Howl, this));
	activitySelector_.push_back(std::bind(&Monster::AttackMode, this));
	activitySelector_.push_back(std::bind(&Monster::WaitingMode, this));
	activitySelector_.push_back(std::bind(&Monster::Dead, this));

	attackSequence_.push_back(std::bind(&Monster::AttackElapsedTime, this));
	attackSequence_.push_back(std::bind(&Monster::AttackModeSelection, this));
	attackSequence_.push_back(std::bind(&Monster::AttackModeMove, this));

	waitingSequence_.push_back(std::bind(&Monster::WaitingElapsedTime, this));
	waitingSequence_.push_back(std::bind(&Monster::WaitingModeSelection, this));
	waitingSequence_.push_back(std::bind(&Monster::WaitingModeMove, this));

	attackSelector_.push_back(std::bind(&Monster::AttackMode1, this));
	attackSelector_.push_back(std::bind(&Monster::AttackMode2, this));
	attackSelector_.push_back(std::bind(&Monster::AttackMode3, this));

	waitingSelector_.push_back(std::bind(&Monster::WaitingMode1, this));
	waitingSelector_.push_back(std::bind(&Monster::WaitingMode2, this));
	waitingSelector_.push_back(std::bind(&Monster::WaitingMode3, this));

	TreeReset();
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	BehaviorTree();

	if (!damageHitFlag_)
	{
		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->SetColor({ 0.4f,0.8f,0.4f,1 });
		}

		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			rightForeFoot_[i]->SetColor({ 1,0.5f,0.25f,1 });
		}

		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			leftForeFoot_[i]->SetColor({ 1,0.5f,0.25f,1 });
		}

		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			rightHindFoot_[i]->SetColor({ 0,0.5f,0.5f,1 });
		}

		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			leftHindFoot_[i]->SetColor({ 0,0.5f,0.5f,1 });
		}

		for (int i = 0; i < tail_.size(); i++)
		{
			tail_[i]->SetColor({ 0.5f,0.5f,0,1 });
		}
	}

	if (!hunter_->GetAnimationType())
	{
		damageHitFlag_ = false;
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

	PartsTailDestruction();
	bloodEmitter_->Update();
	bubbleEmitter_->Update();
}

void Monster::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (hp_ >= 1)
	{
		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->Draw(cmdList);
		}
		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			rightForeFoot_[i]->Draw(cmdList);
		}
		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			leftForeFoot_[i]->Draw(cmdList);
		}
		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			rightHindFoot_[i]->Draw(cmdList);
		}
		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			leftHindFoot_[i]->Draw(cmdList);
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			tail_[i]->Draw(cmdList);
		}
	}
	bloodEmitter_->Draw(cmdList);
	bubbleEmitter_->Draw(cmdList);
}

void Monster::AngleAdjustment()
{
	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };
	XMFLOAT3 enemyRot = nucleus_->GetRotation();

	enemyRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (ANGLE / PI);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(result.y - 0.0f, result.x - 0.0f) * (ANGLE / PI);

	float timeRate = 0.0f;
	int countNum = 30;
	timeRate = angleEaseTimer_ / countNum;
	angleEaseTimer_++;

	nucleus_->SetRotation(Ease::easeInOut(nucleus_->GetRotation(), enemyRot, timeRate));

	if (angleEaseTimer_ > countNum)
	{
		nucleus_->SetRotation(enemyRot);
	}
}

bool Monster::Hit(XMFLOAT3 partsPosition, float enemyRange, float playerRange)
{
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { partsPosition.x, partsPosition.y, partsPosition.z, 1.0f };
	eSphere.radius = enemyRange;
	pSphere.center = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z, 1.0f };
	pSphere.radius = playerRange;

	if (Collision::CheckSphere2Sphere(eSphere, pSphere))
	{
		return true;
	}
	return false;
}

void Monster::DamageHit(Sphere hitSphere)
{
	Sphere eSphere;

	int count = 15;
	int life = 60;
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 体
	for (int i = 0; i < body_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			body_[i]->GetWorldPosition().x,
			body_[i]->GetWorldPosition().y,
			body_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			hp_ -= (float)PartsDamage::Body * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, body_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < body_.size(); i++)
			{
				body_[i]->SetColor(color);
			}
		}
	}

	// 右前足
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			rightForeFoot_[i]->GetWorldPosition().x,
			rightForeFoot_[i]->GetWorldPosition().y,
			rightForeFoot_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			hp_ -= (float)PartsDamage::RightForeFoot * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, rightForeFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < rightForeFoot_.size(); i++)
			{
				rightForeFoot_[i]->SetColor(color);
			}
		}
	}

	// 左前足
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			leftForeFoot_[i]->GetWorldPosition().x,
			leftForeFoot_[i]->GetWorldPosition().y,
			leftForeFoot_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			hp_ -= (float)PartsDamage::LeftForeFoot * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, leftForeFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < leftForeFoot_.size(); i++)
			{
				leftForeFoot_[i]->SetColor(color);
			}
		}
	}

	// 右後足
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			rightHindFoot_[i]->GetWorldPosition().x,
			rightHindFoot_[i]->GetWorldPosition().y,
			rightHindFoot_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			hp_ -= (float)PartsDamage::RightHindFoot * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, rightHindFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < rightHindFoot_.size(); i++)
			{
				rightHindFoot_[i]->SetColor(color);
			}
		}
	}

	// 左後足
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			leftHindFoot_[i]->GetWorldPosition().x,
			leftHindFoot_[i]->GetWorldPosition().y,
			leftHindFoot_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			hp_ -= (float)PartsDamage::LeftHindFoot * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, leftHindFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < leftHindFoot_.size(); i++)
			{
				leftHindFoot_[i]->SetColor(color);
			}
		}
	}

	// 尻尾
	for (int i = 0; i < tail_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		eSphere.center =
		{
			tail_[i]->GetWorldPosition().x,
			tail_[i]->GetWorldPosition().y,
			tail_[i]->GetWorldPosition().z, 1
		};

		if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
		{
			damageHitFlag_ = true;
			tailDestruction_ += 10;
			hp_ -= (float)PartsDamage::Tail * ItemManager::GetInstance()->AttackBuffMagnification();
			bloodEmitter_->BloodAdd(count, life, tail_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));

			for (int i = 0; i < tail_.size(); i++)
			{
				tail_[i]->SetColor(color);
			}
		}
	}
}

void Monster::Animation(AnimationType type)
{
	int count = 10;
	int life = 60;

	// 初期姿勢
	if (type == AnimationType::InitialPosture)
	{
		XMFLOAT3 rot = {};

		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->SetRotation(rot);
		}
		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			rightForeFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			leftForeFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			rightHindFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			leftHindFoot_[i]->SetRotation(rot);
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			if (!tailDestructionFlag_)
			{
				tail_[i]->SetRotation(rot);
			}
		}

		for (int i = 0; i < body_.size(); i++)
		{
			if (i == 0)
			{
				body_[i]->SetPosition({});
			}
			else
			{
				body_[i]->SetPosition({ -1.5f,0,0 });
			}
		}
		rightForeFoot_[0]->SetPosition({ 0,0,-1 });
		rightForeFoot_[1]->SetPosition({ 0,-1.0f,-1 });
		rightForeFoot_[2]->SetPosition({ 1,-1.0f,0 });

		leftForeFoot_[0]->SetPosition({ 0,0,1 });
		leftForeFoot_[1]->SetPosition({ 0,-1.0f,1 });
		leftForeFoot_[2]->SetPosition({ 1,-1.0f,0 });

		rightHindFoot_[0]->SetPosition({ -5.5f,0,-1 });
		rightHindFoot_[1]->SetPosition({ 0,-1.0f,-1 });
		rightHindFoot_[2]->SetPosition({ -1,-1.0f,0 });

		leftHindFoot_[0]->SetPosition({ -5.5f,0,1 });
		leftHindFoot_[1]->SetPosition({ 0,-1.0f,1 });
		leftHindFoot_[2]->SetPosition({ -1,-1.0f,0 });

		for (int i = 0; i < tail_.size(); i++)
		{
			if (!tailDestructionFlag_)
			{
				tail_[i]->SetPosition({ -1.5f,0,0 });
			}
		}
	}
	// 歩行
	else if (type == AnimationType::Move)
	{
		// 幅の制限
		float restrictionAngle = 135.0f;

		XMFLOAT3 rotA = rightForeFoot_[0]->GetRotation();
		rotA.z -= addAngle;
		rightForeFoot_[0]->SetRotation(rotA);
		leftHindFoot_[0]->SetRotation(rotA);

		XMFLOAT3 rotB = leftForeFoot_[0]->GetRotation();
		rotB.z += addAngle;
		leftForeFoot_[0]->SetRotation(rotB);
		rightHindFoot_[0]->SetRotation(rotB);

		if (rotB.z > 45.0f)
		{
			addAngle = -5.0f;
		}
		else if (rotB.z < -45.0f)
		{
			addAngle = 5.0f;
		}

		for (auto& a : tail_)
		{
			XMFLOAT3 tailRot = a->GetRotation();

			tailRot.z += cosf(PI * 2.0f / restrictionAngle * (waveTimer_));

			if (!tailDestructionFlag_)
			{
				a->SetRotation(tailRot);
			}
		}

		waveTimer_++;
	}
	//突進
	else if (type == AnimationType::Assault)
	{
		XMFLOAT3 bodyRot = body_[0]->GetRotation();
		float addAngle = 10.0f;

		bodyRot.x += addAngle;

		if (bodyRot.x >= 360.0f)
		{
			bodyRot.x = 0.0f;
		}

		body_[0]->SetRotation(bodyRot);

		XMFLOAT3 rot = rightForeFoot_[0]->GetRotation();

		rot.z -= addAngle;

		if (rot.z <= -90.0f)
		{
			rot.z = -90.0f;
		}

		rightForeFoot_[0]->SetRotation(rot);
		leftForeFoot_[0]->SetRotation(rot);
		leftHindFoot_[0]->SetRotation(rot);
		rightHindFoot_[0]->SetRotation(rot);
		bubbleEmitter_->SetCenter(5);
		
		bubbleEmitter_->BubbleAdd(count, life, rightForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		bubbleEmitter_->BubbleAdd(count, life, leftForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		bubbleEmitter_->BubbleAdd(count, life, leftHindFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		bubbleEmitter_->BubbleAdd(count, life, rightHindFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	}
	//尻尾攻撃
	else if (type == AnimationType::TailAttack)
	{
		float timeRate = 0.0f;
		int countNum = 30;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		body_[0]->SetRotation(Ease::easeInOut({}, { 0.0f,360.0f,0.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			isEaseFlag_ = true;
		}

		bubbleEmitter_->SetCenter(1);
		for (auto& a : tail_)
		{
			bubbleEmitter_->BubbleAdd(count, life, a->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		}
	}
	//パンチ攻撃
	else if (type == AnimationType::Punch)
	{
		float timeRate = 0.0f;

		if (!isPunch)
		{
			int countNum = 20;
			timeRate = easeTimer_ / countNum;
			easeTimer_++;

			rightForeFoot_[0]->SetRotation(Ease::easeIn({}, { 0.0f, 45.0f, 100.0f }, timeRate));

			if (easeTimer_ > countNum)
			{
				easeTimer_ = 0.0f;
				isPunch = true;
			}
		}
		else if (isPunch)
		{
			int countNum = 10;
			timeRate = easeTimer_ / countNum;
			easeTimer_++;

			rightForeFoot_[0]->SetRotation(Ease::easeOut({ 0.0f, 45.0f, 100.0f }, { 0.0f,-90.0f,0.0f }, timeRate));

			if (easeTimer_ > countNum)
			{
				easeTimer_ = 0.0f;
				isPunch = false;
				isEaseFlag_ = true;
			}
			bubbleEmitter_->SetCenter(1);
			bubbleEmitter_->BubbleAdd(count, life, rightForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		}
	}
	// 待機
	else if (type == AnimationType::Waiting)
	{
		float timeRate = 0.0f;
		int countNum = 30;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		float range = 0.3f;

		if (!isEaseFlag_)
		{
			body_[0]->SetPosition(Ease::easeInOut({ 0.0f, 0.0f + range, 0.0f }, { 0.0f, 0.0f - range, 0.0f }, timeRate));

			rightForeFoot_[0]->SetPosition(Ease::easeInOut({ 0,0,-1 }, { 0, range, -1 }, timeRate));
			leftForeFoot_ [0]->SetPosition(Ease::easeInOut( { 0,0,1 }, { 0, range, 1 }, timeRate));
			leftHindFoot_ [0]->SetPosition(Ease::easeInOut({ -5.5f,0,1 }, { -5.5f,range,1 }, timeRate));
			rightHindFoot_[0]->SetPosition(Ease::easeInOut({ -5.5f,0,-1 }, { -5.5f,range,-1 }, timeRate));

			if (easeTimer_ > countNum)
			{
				easeTimer_ = 0.0f;
				isEaseFlag_ = true;
			}
		}
		else if (isEaseFlag_)
		{
			body_[0]->SetPosition(Ease::easeInOut({ 0.0f, 0.0f - range, 0.0f }, { 0.0f, 0.0f + range, 0.0f }, timeRate));

			rightForeFoot_[0]->SetPosition(Ease::easeInOut({ 0, range, -1 }, { 0, 0, -1 }, timeRate));
			leftForeFoot_ [0]->SetPosition(Ease::easeInOut({ 0, range, 1 }, { 0, 0, 1 }, timeRate));
			leftHindFoot_ [0]->SetPosition(Ease::easeInOut({ -5.5f,range,1 }, { -5.5f,0,1 }, timeRate));
			rightHindFoot_[0]->SetPosition(Ease::easeInOut({ -5.5f,range,-1 }, { -5.5f,0,-1 }, timeRate));

			if (easeTimer_ > countNum)
			{
				easeTimer_ = 0.0f;
				isEaseFlag_ = false;
			}
		}
	}
}

void Monster::PartsTailDestruction()
{
	if (tailDestruction_ >= 150 && !tailDestructionFlag_)
	{
		tail_[0]->SetParent((Object3d*)nullptr);

		XMFLOAT3 ppos = tail_[0]->GetPosition();
		XMFLOAT3 prot = tail_[0]->GetRotation();

		ppos.x += body_[4]->GetWorldPosition().x;
		ppos.y += body_[4]->GetWorldPosition().y;
		ppos.z += body_[4]->GetWorldPosition().z;

		prot.x += nucleus_->GetRotation().x;
		prot.y += nucleus_->GetRotation().y;
		prot.z += nucleus_->GetRotation().z;

		tail_[0]->SetPosition(ppos);
		tail_[0]->SetRotation(prot);

		tailDestructionFlag_ = true;
	}
}

void Monster::BehaviorTree()
{
	for (auto& a : activitySelector_)
	{
		if (a())
		{
			break;
		}
	}
}

bool Monster::Howl()
{
	if (howlflag_)
	{
		return false;
	}

	howlflag_ = true;

	return true;
}

bool Monster::AttackMode()
{
	for (auto& a : attackSequence_)
	{
		if (!a())
		{
			return false;
		}
	}

	return true;
}

bool Monster::WaitingMode()
{
	for (auto& a : waitingSequence_)
	{
		if (!a())
		{
			return false;
		}
	}

	return true;
}

bool Monster::Dead()
{
	if (hp_ >= 1.0f)
	{
		TreeReset();
	}
	else
	{
		isDead_ = true;
	}

	return true;
}

bool Monster::AttackElapsedTime()
{
	if (attackEnd_)
	{
		return false;
	}

	if (attackElapsedTimer_ >= 60 && (body_[0]->GetRotation().x <= 0.0f && attackSelect_[0]) ||
		attackSelect_[1] && isEaseFlag_ || attackSelect_[2] && isEaseFlag_)
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		hitFlag_ = false;
		attackEnd_ = true;
		return false;
	}

	if (trackingEnd_)
	{
		attackElapsedTimer_++;
	}

	return true;
}

bool Monster::AttackModeSelection()
{
	if (attackSelect_[0] || attackSelect_[1] || attackSelect_[2])
	{
		return true;
	}

	if (!Hit(body_[2]->GetWorldPosition(), 1.0f, 50.0f))
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		attackSelect_[0] = true;
		return true;
	}
	else if (Hit(body_[2]->GetWorldPosition(), 15.0f, 1.0f) && !Hit(body_[2]->GetWorldPosition(), 8.0f, 1.0f))
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		attackSelect_[1] = true;
		return true;
	}
	else if (Hit(body_[2]->GetWorldPosition(), 4.5f, 1.0f))
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		attackSelect_[2] = true;
		return true;
	}

	return false;
}

bool Monster::AttackModeMove()
{
	for (int i = 0; i < attackSelector_.size(); i++)
	{
		if (attackSelect_[i])
		{
			return attackSelector_[i]();
		}
	}

	return false;
}

bool Monster::AttackMode1()
{
	Animation(AnimationType::Assault);

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 1.0f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	if (!trackingEnd_)
	{
		AngleAdjustment();
		saveVector_ = vector;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 20.0f))
	{
		trackingEnd_ = true;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
	{
		hunter_->SetDamageFlag(true);
		hunter_->SetDamage(10.0f);
		hitFlag_ = true;
	}

	pos.x += saveVector_.x;
	pos.y += saveVector_.y;
	pos.z += saveVector_.z;

	if (pos.x <= -48.0f)
	{
		pos.x = -48.0f;
	}
	else if (pos.x >= 48.0f)
	{
		pos.x = 48.0f;
	}

	if (pos.y <= 1.0f)
	{
		pos.y = 1.0f;
	}
	else if (pos.y >= 58.0f)
	{
		pos.y = 58.0f;
	}

	if (pos.z <= -48.0f)
	{
		pos.z = -48.0f;
	}
	else if (pos.z >= 48.0f)
	{
		pos.z = 48.0f;
	}

	nucleus_->SetPosition(pos);

	return true;
}

bool Monster::AttackMode2()
{
	Animation(AnimationType::TailAttack);
	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	for (int i = 0; i < tail_.size(); i++)
	{
		if (hitFlag_)
		{
			continue;
		}
		if (Hit(tail_[i]->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10);
			hitFlag_ = true;
		}
	}

	return true;
}

bool Monster::AttackMode3()
{
	Animation(AnimationType::Punch);
	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		if (hitFlag_)
		{
			continue;
		}
		if (Hit(rightForeFoot_[i]->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10);
			hitFlag_ = true;
		}
	}

	return true;
}

bool Monster::WaitingElapsedTime()
{
	if (waitingElapsedTimer_ >= 60 || waitingEnd_)
	{
		if (count >= maxCount)
		{
			count = 0;
			maxCount = RandCalculate(1.0f, 4.0f);
		}
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		return false;
	}

	waitingElapsedTimer_++;

	return true;
}

bool Monster::WaitingModeSelection()
{
	if (waitingSelect_[0] || waitingSelect_[1] || waitingSelect_[2])
	{
		return true;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 1.0f) || count >= maxCount)
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		waitingSelect_[0] = true;
		return true;
	}
	else if (!Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		waitingSelect_[1] = true;
		return true;
	}
	else if (Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		Animation(AnimationType::InitialPosture);
		angleEaseTimer_ = 0.0f;
		waitingSelect_[2] = true;
		return true;
	}

	return false;
}

bool Monster::WaitingModeMove()
{
	for (int i = 0; i < waitingSelector_.size(); i++)
	{
		if (waitingSelect_[i])
		{
			return waitingSelector_[i]();
		}
	}

	return false;
}

bool Monster::WaitingMode1()
{
	Animation(AnimationType::Waiting);
	return true;
}

bool Monster::WaitingMode2()
{
	Animation(AnimationType::Move);
	AngleAdjustment();

	int countNum = 30;
	if (angleEaseTimer_ > countNum)
	{
		waitingEnd_ = true;
		return false;
	}

	if (Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		return true;
	}

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 0.8f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	if (pos.x <= -48.0f)
	{
		pos.x = -48.0f;
	}
	else if (pos.x >= 48.0f)
	{
		pos.x = 48.0f;
	}

	if (pos.y <= 1.0f)
	{
		pos.y = 1.0f;
	}
	else if (pos.y >= 58.0f)
	{
		pos.y = 58.0f;
	}

	if (pos.z <= -48.0f)
	{
		pos.z = -48.0f;
	}
	else if (pos.z >= 48.0f)
	{
		pos.z = 48.0f;
	}

	nucleus_->SetPosition(pos);

	return true;
}

bool Monster::WaitingMode3()
{
	if (!Hit(body_[2]->GetWorldPosition(), 80.0f, 1.0f))
	{
		waitingEnd_ = true;
		return false;
	}

	Animation(AnimationType::Move);
	AngleAdjustment();

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = -1.0f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	if (pos.x <= -48.0f)
	{
		pos.x = -48.0f;
	}
	else if (pos.x >= 48.0f)
	{
		pos.x = 48.0f;
	}

	if (pos.y <= 1.0f)
	{
		pos.y = 1.0f;
	}
	else if (pos.y >= 58.0f)
	{
		pos.y = 58.0f;
	}

	if (pos.z <= -48.0f)
	{
		pos.z = -48.0f;
	}
	else if (pos.z >= 48.0f)
	{
		pos.z = 48.0f;
	}

	nucleus_->SetPosition(pos);

	return true;
}

void Monster::TreeReset()
{
	attackSelect_.clear();
	for (int i = 0; i < attackSelector_.size(); i++)
	{
		bool flag = false;
		attackSelect_.push_back(flag);
	}

	waitingSelect_.clear();
	for (int i = 0; i < waitingSelector_.size(); i++)
	{
		bool flag = false;
		waitingSelect_.push_back(flag);
	}

	isEaseFlag_ = false;
	easeTimer_ = 0.0f;
	attackElapsedTimer_ = 0;
	waitingElapsedTimer_ = 0;

	trackingEnd_ = false;
	attackEnd_ = false;
	waitingEnd_ = false;

	count++;
}
