#include "Monster.h"
#include "FbxFactory.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "DebugText.h"
#include "ObjFactory.h"
#include "ItemManager.h"

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
			body_[i]->SetParent(body_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 5Ui64>::size_type>(i) - 1].get());
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
			leftForeFoot_[i]->SetPosition({ -1.5f,0,1 });
			leftForeFoot_[i]->SetParent(body_[0].get());
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

	testBlood_ = std::make_unique<ObjParticle>();
	blood_ = std::make_unique<ParticleEmitter>(testBlood_.get());
	blood_->SetCenter(1);
	blood_->SetObjScale({ 0.5f, 0.5f, 0.5f });
	blood_->SetStartColor({ 1,0,0,1 });
	blood_->SetEndColor({ 1,0,0,1 });


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

	TreeReset();
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	BehaviorTree();

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
	blood_->Update();
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
	blood_->Draw(cmdList);
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

void Monster::DamageHit(Sphere hitSphere)
{
	Sphere eSphere;

	int count = 15;
	int life = 120;

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
			blood_->Add(count, life, body_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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
			blood_->Add(count, life, rightForeFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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
			blood_->Add(count, life, leftForeFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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
			blood_->Add(count, life, rightHindFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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
			blood_->Add(count, life, leftHindFoot_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
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
			blood_->Add(count, life, tail_[i]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("sphere"));
		}
	}
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
			if (!tailDestructionFlag_)
			{
				tail_[i]->SetRotation(rot);
			}
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

			tailRot.z += cosf(PI * 2 / 70 * (waveTimer_ / 2));

			if (!tailDestructionFlag_)
			{
				tail_[i]->SetRotation(tailRot);
			}
		}

		waveTimer_ += 4;
	}
	//突進
	else if (type == AnimationType::Assault)
	{
		XMFLOAT3 bodyRot = body_[0]->GetRotation();

		bodyRot.x += MAX_ANGLE;

		if (bodyRot.x >= 360)
		{
			bodyRot.x = 0;
		}

		body_[0]->SetRotation(bodyRot);

		XMFLOAT3 rot = rightForeFoot_[0]->GetRotation();

		rot.z -= 10;

		if (rot.z <= -90)
		{
			rot.z = -90;
		}

		rightForeFoot_[0]->SetRotation(rot);
		leftForeFoot_[0]->SetRotation(rot);
		leftHindFoot_[0]->SetRotation(rot);
		rightHindFoot_[0]->SetRotation(rot);
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
	for (auto& a: activitySelector_)
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
	if (attackEnd_)
	{
		Animation(AnimationType::Stop);
		hitFlag_ = false;
		return false;
	}

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
	if (hp_ >= 1)
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
	if (attackElapsedTimer_ >= 60 && body_[0]->GetRotation().x <= 0)
	{
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
	if (attackSelector_[0])
	{
		return true;
	}

	for (int i = 0; i < attackSelector_.size(); i++)
	{
		if (i == 0 && !Hit(nucleus_->GetPosition(), 1.0f, 50))
		{
			attackSelector_[i] = true;
			return true;
		}
	}

	return false;
}

bool Monster::AttackModeMove()
{
	if (attackSelector_[0])
	{
		return AttackMode1();
	}
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

	if (Hit(nucleus_->GetPosition(), 1.0f, 20))
	{
		trackingEnd_ = true;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
	{
		hunter_->SetDamageFlag(true);
		hunter_->SetDamage(10);
		hitFlag_ = true;
	}

	pos.x += saveVector_.x;
	pos.y += saveVector_.y;
	pos.z += saveVector_.z;

	if (pos.y <= 1)
	{
		pos.y = 1;
	}

	nucleus_->SetPosition(pos);

	return true;
}

bool Monster::WaitingElapsedTime()
{
	if (waitingElapsedTimer_ >= 60 || waitingEnd_)
	{
		return false;
	}

	waitingElapsedTimer_++;

	return true;
}

bool Monster::WaitingModeSelection()
{
	if (waitingSelector_[0] || waitingSelector_[1] || waitingSelector_[2])
	{
		return true;
	}

	for (int i = 0; i < waitingSelector_.size(); i++)
	{
		if (i == 0 && Hit(nucleus_->GetPosition(), 1.0f, 1.0f))
		{
			waitingSelector_[i] = true;
			return true;
		}
		if (i == 1 && !Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
		{
			waitingSelector_[i] = true;
			return true;
		}
		if (i == 2 && Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
		{
			waitingSelector_[i] = true;
			return true;
		}
	}

	return false;
}

bool Monster::WaitingModeMove()
{
	if (waitingSelector_[0])
	{
		return WaitingMode1();
	}
	else if (waitingSelector_[1])
	{
		return WaitingMode2();
	}
	else if (waitingSelector_[2])
	{
		return WaitingMode3();
	}

	return false;
}

bool Monster::WaitingMode1()
{
	Animation(AnimationType::Stop);
	return true;
}

bool Monster::WaitingMode2()
{
	if (Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		waitingEnd_ = true;
		return false;
	}

	Animation(AnimationType::Move);
	AngleAdjustment();

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 0.8f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	if (pos.y <= 1)
	{
		pos.y = 1;
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

	if (pos.y <= 1)
	{
		pos.y = 1;
	}

	nucleus_->SetPosition(pos);

	return true;
}

void Monster::TreeReset()
{
	attackSelector_.clear();
	for (int i = 0; i < 1; i++)
	{
		bool flag = false;
		attackSelector_.push_back(flag);
	}
	
	waitingSelector_.clear();
	for (int i = 0; i < 3; i++)
	{
		bool flag = false;
		waitingSelector_.push_back(flag);
	}

	attackElapsedTimer_ = 0;
	waitingElapsedTimer_ = 0;

	trackingEnd_ = false;
	attackEnd_ = false;
	waitingEnd_ = false;
}
