#include "Hunter.h"
#include "FbxFactory.h"
#include "ObjFactory.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Vector.h"
#include "DebugText.h"
#include "ItemManager.h"

std::unique_ptr<Hunter> Hunter::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Hunter* fbxObject3d = new Hunter();
	if (fbxObject3d == nullptr)
	{
		return nullptr;
	}
	fbxObject3d->Initialize();

	return std::unique_ptr<Hunter>(fbxObject3d);
}

void Hunter::Initialize()
{
	// �v���C���[�֌W
	float size = 0.006f;
	hunter_[0] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("halt"),   L"BasicFBX", true);
	hunter_[1] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("move"),   L"BasicFBX", true);
	hunter_[2] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("damage"), L"BasicFBX", true);
	hunter_[3] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("death"),  L"BasicFBX", true);
	hunter_[4] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("dash"),   L"BasicFBX", true);
	hunter_[5] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("dodge"),  L"BasicFBX", true);
	hunter_[6] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("combo1"), L"BasicFBX", true);
	hunter_[7] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("combo2"), L"BasicFBX", true);
	hunter_[8] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("combo3"), L"BasicFBX", true);

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->SetScale({ size, size, size });
		hunter_[i]->SetRotation({ 0.0f,0.0f,0.0f });
		hunter_[i]->SetPosition({ 0.0f, 10.0f, -30.0f });
	}
	falg_.halt = true;
	hunter_[animationType_]->PlayAnimation();

	// ����֌W
	weapon_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("katana"));
	float s = 0.05f;
	weapon_->SetScale({ s,s,s });
	weapon_->SetBoneName("mixamorig:LeftHandThumb4");
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });

	// �A�C�e���p�̃p�[�e�B�N��
	itemParticle_ = std::make_unique<ObjParticle>();
	itemParticle_->SetParent(hunter_[animationType_].get());
	itemEmitter_ = std::make_unique<ParticleEmitter>(itemParticle_.get());
	itemEmitter_->SetCenter({ 2.0f, 1.0f, 2.0f });
	float scale = 0.1f;
	itemEmitter_->SetObjStartScale({ scale, scale, scale });

	healParticle_ = std::make_unique<ObjParticle>();
	healParticle_->SetParent(hunter_[animationType_].get());
	healEmitter_ = std::make_unique<ParticleEmitter>(healParticle_.get());
	healEmitter_->SetCenter({ 2.0f, 1.0f, 2.0f });
	healEmitter_->SetObjStartScale({ scale, scale, scale });
	healEmitter_->SetStartColor({ 0.2f, 1.0f, 0.0f, 1.0f });
	healEmitter_->SetEndColor({ 0.2f, 1.0f, 0.0f, 1.0f });

	animationEaseData_ = std::make_unique<EaseData>(40);
	animationEaseData_->SetActFlag(false);

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->SetEaseData(animationEaseData_.get());
	}

	// �i�K�̍\�z
	animationFunc_.push_back(std::bind(&Hunter::Halt, this));
	animationFunc_.push_back(std::bind(&Hunter::Move, this));
	animationFunc_.push_back(std::bind(&Hunter::Damage, this));
	animationFunc_.push_back(std::bind(&Hunter::Death, this));
	animationFunc_.push_back(std::bind(&Hunter::Dash, this));
	animationFunc_.push_back(std::bind(&Hunter::Dodge, this));
	animationFunc_.push_back(std::bind(&Hunter::Combo1, this));
	animationFunc_.push_back(std::bind(&Hunter::Combo2, this));
	animationFunc_.push_back(std::bind(&Hunter::Combo3, this));
}

void Hunter::Finalize()
{
}

void Hunter::Update()
{
	if (!falg_.death)
	{
		ItemUse();
		DamageHit();
	}

	hunter_[animationType_]->Update();
	weapon_->Update();
	itemEmitter_->Update();
	healEmitter_->Update();
}

void Hunter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	hunter_[animationType_]->Draw(cmdList);

	weapon_->Draw(cmdList);
	itemEmitter_->Draw(cmdList);
	healEmitter_->Draw(cmdList);
}

void Hunter::Behavior()
{
	if (hp_ >= 1.0f)
	{
		// �ړ�
		BaseMove();
		// ���
		AvoidMove();
		// �U��
		AttackMove();
	}
	else
	{
		if (!falg_.death)
		{
			animationFunc_[static_cast<int>(AnimationType::DEATH)]();
		}
	}
}

void Hunter::BaseMove()
{
	Input* input = Input::GetInstance();

	XMFLOAT3 position = hunter_[animationType_]->GetPosition();
	XMFLOAT3 rotation = hunter_[animationType_]->GetRotation();

	if (((input->PadStickGradient().x <= -ANGLE_RESTRICTION || input->PadStickGradient().x >= ANGLE_RESTRICTION) ||
		(input->PadStickGradient().y <= -ANGLE_RESTRICTION || input->PadStickGradient().y >= ANGLE_RESTRICTION)) && !isAttackFlag_ && !falg_.damage)
	{
		// �X�s�[�h�v�Z
		SpeedCalculate();

		playerAngle_ = { cameraAngle_.x + static_cast<float>(input->PadStickAngle()), cameraAngle_.y };

		if (input->PadStickGradient().y < 0.0f || cameraAngle_.y < 0.0f || input->PadStickGradient().y > 0.0f && cameraAngle_.y > 0.0f)
		{
			speed_.y *= -1.0f;
		}

		if (input->PadStickGradient().y > 0.0f)
		{
			playerAngle_.y = -playerAngle_.y;
		}

		rotation.y = playerAngle_.x + 90;
		rotation.x = playerAngle_.y;

		if (isDash_ && !falg_.dash && !falg_.dodge)
		{
			animationFunc_[static_cast<int>(AnimationType::DASH)]();
		}
		else if (!falg_.move && !isDash_ && !falg_.dodge)
		{
			animationFunc_[static_cast<int>(AnimationType::MOVE)]();
		}
	}
	else if (!isAttackFlag_ && !falg_.damage && !falg_.dodge)
	{
		if (!falg_.halt)
		{
			animationFunc_[static_cast<int>(AnimationType::HALT)]();
		}
	}

	position.x +=  cosf((playerAngle_.x * 3.14f) / 180.0f) * speed_.x;
	position.y +=  sinf((playerAngle_.y * 3.14f) / 180.0f) * speed_.y;
	position.z += -sinf((playerAngle_.x * 3.14f) / 180.0f) * speed_.x;

	if (position.x <= -48.0f)
	{
		position.x = -48.0f;
	}
	else if (position.x >= 48.0f)
	{
		position.x = 48.0f;
	}

	if (position.y <= 1.0f)
	{
		position.y = 1.0f;
	}
	else if (position.y >= 58.0f)
	{
		position.y = 58.0f;
	}

	if (position.z <= -48.0f)
	{
		position.z = -48.0f;
	}
	else if (position.z >= 48.0f)
	{
		position.z = 48.0f;
	}

	if (speed_.x != 0.0f)
	{
		speed_.x *= 0.925f;
	}
	if (speed_.y != 0.0f)
	{
		speed_.y *= 0.925f;
	}

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->SetPosition(position);
		hunter_[i]->SetRotation(rotation);
	}
}

void Hunter::AvoidMove()
{
	Input* input = Input::GetInstance();

	// ���
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10 && isStamina_ && !falg_.damage && !isAttackFlag_ && (input->PadStickGradient().x != 0.0f || input->PadStickGradient().y != 0.0f))
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;

		if (!falg_.dodge)
		{
			animationFunc_[static_cast<int>(AnimationType::DODGE)]();
		}
	}

	if (hunter_[animationType_]->AnimationEnd())
	{
		falg_.dodge = false;
	}
}

void Hunter::AttackMove()
{
	Input* input = Input::GetInstance();

	// �U��
	if ((input->TriggerPadKey(BUTTON_Y) || input->TriggerPadKey(BUTTON_B)) && !avoidFlag_ &&  attackCoolTimer_ >= 10 && !itemSelectionFlag_ && !falg_.damage)
	{
		attackCoolTimer_ = 0;
		comboFlag_ = true;
	}

	// �R���{���Ă�����
	if (comboFlag_)
	{
		isAttackFlag_ = true;
		attackCoolTimer_ = 0;
		if (falg_.attack1 && falg_.attack2 && !falg_.attack3 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			animationFunc_[static_cast<int>(AnimationType::COMBO3)]();
		}
		else if (falg_.attack1 && !falg_.attack2 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			animationFunc_[static_cast<int>(AnimationType::COMBO2)]();
		}
		else if (!falg_.attack1 || falg_.attack1 && falg_.attack2 && falg_.attack3 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			animationFunc_[static_cast<int>(AnimationType::COMBO1)]();
		}
	}

	if (isAttackFlag_)
	{
		if (attackCoolTimer_ >= 10 && hunter_[animationType_]->AnimationEnd() && !comboFlag_)
		{
			isAttackFlag_ = false;
			attackCoolTimer_ = 0;
		}
	}
	attackCoolTimer_++;
}

void Hunter::SpeedCalculate()
{
	Input* input = Input::GetInstance();

	if (avoidFlag_ || falg_.dodge)
	{
		if (avoidTimer_ <= 0)
		{
			stamina_ -= 10.0f;
		}

		avoidTimer_++;

		speed_.x = speed_.y = static_cast<float>(sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) * 1.2f);

		if (avoidTimer_ >= 10 && !falg_.dodge)
		{
			avoidTimer_ = 0;
			avoidFlag_ = false;
		}
		isDash_ = false;
	}
	else if (input->PushPadKey(BUTTON_RIGHT_SHOULDER) && isStamina_)
	{
		speed_.x = speed_.y = static_cast<float>(sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y));
		stamina_ -= 0.5f;
		avoidTimer_++;
		isDash_ = true;
	}
	else
	{
		speed_.x = speed_.y = static_cast<float>(sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) / 2.0f);
		avoidTimer_++;
		isDash_ = false;
	}

	if (isStamina_ && stamina_ <= 0.0f)
	{
		stamina_ = 0.0f;
		isStamina_ = false;
	}
	else if (!isStamina_ && stamina_ >= 50.0f)
	{
		isStamina_ = true;
	}
}

void Hunter::ItemUse()
{
	Input* input = Input::GetInstance();

	// �A�C�e���̑I��
	if (input->PushPadKey(BUTTON_LEFT_SHOULDER))
	{
		if (input->TriggerPadKey(BUTTON_X) && isItemSelectionEase_)
		{
			itemType_++;
			isItemSelectionEase_ = false;
			buttonFlag_ = true;
		}
		else if (input->TriggerPadKey(BUTTON_B) && isItemSelectionEase_)
		{
			itemType_--;
			isItemSelectionEase_ = false;
			buttonFlag_ = false;
		}

		if (itemType_ > 2)
		{
			itemType_ = 0;
		}
		else if (itemType_ < 0)
		{
			itemType_ = 2;
		}

		itemSelectionFlag_ = true;
	}
	else
	{
		itemSelectionFlag_ = false;
	}

	// �A�C�e���̎g�p
	itemParticle_->SetParent(hunter_[animationType_].get());
	healParticle_->SetParent(hunter_[animationType_].get());
	if (input->TriggerPadKey(BUTTON_X) && !itemSelectionFlag_ && !buttonEaseFlag_)
	{
		int count = ItemManager::GetInstance()->GetItemQuantity(itemType_);
		buttonEaseFlag_ = true;

		if (ItemManager::GetInstance()->GetItemType(itemType_) == ItemManager::ItemType::Healing && count > 0 && hp_ < MAX_HP)
		{
			count--;
			hp_ += 10.0f;
			if (hp_ >= MAX_HP)
			{
				hp_ = MAX_HP;
			}
			if (animationType_ != 1)
			{
				healEmitter_->HealAdd(3, 30, {}, ObjFactory::GetInstance()->GetModel("heal"), 0.1f);
				healEmitter_->HealAdd(7, 30, {}, ObjFactory::GetInstance()->GetModel("itemEffect"));
			}
			else
			{
				healEmitter_->HealAdd(3, 30, { 0.0f, -0.5f, 0.0f }, ObjFactory::GetInstance()->GetModel("heal"), 0.3f);
				healEmitter_->HealAdd(7, 30, { 0.0f, -0.5f, 0.0f }, ObjFactory::GetInstance()->GetModel("itemEffect"));
			}
		}
		else if (ItemManager::GetInstance()->GetItemType(itemType_) == ItemManager::ItemType::AttackBuff && count > 0)
		{
			count--;
			ItemManager::GetInstance()->SetAttackBuff(true, 0, 0, 1.5f);
		}
		else if (ItemManager::GetInstance()->GetItemType(itemType_) == ItemManager::ItemType::DefenseBuff && count > 0)
		{
			count--;
			ItemManager::GetInstance()->SetDefenseBuff(true, 0, 0, 2.0f);
		}

		ItemManager::GetInstance()->SetItemQuantity(itemType_, count);
	}

	// �p�[�e�B�N���̐�
	int count = 4;
	if (ItemManager::GetInstance()->IsAttackBuff() && ItemManager::GetInstance()->IsDefenseBuff())
	{
		count = 2;
	}

	// �U���p�[�e�B�N��
	if (ItemManager::GetInstance()->IsAttackBuff())
	{
		attack.timer++;

		if (attack.timer > attack.count * 20)
		{
			itemEmitter_->SetStartColor({ 1.0f, 0.2f, 0.0f, 1.0f });
			itemEmitter_->SetEndColor({ 1.0f, 0.2f, 0.0f, 1.0f });
			itemEmitter_->ItemAdd(count, 30, { 0.0f, -1.2f, 0.0f }, ObjFactory::GetInstance()->GetModel("itemEffect"));
			attack.count++;
		}
	}
	else
	{
		attack.timer = 0;
		attack.count = 0;
	}
	// �h��p�[�e�B�N��
	if (ItemManager::GetInstance()->IsDefenseBuff())
	{
		defense.timer++;

		if (defense.timer > defense.count * 20)
		{
			itemEmitter_->SetStartColor({ 1.0f, 0.6f, 0.0f, 1.0f });
			itemEmitter_->SetEndColor({ 1.0f, 0.6f, 0.0f, 1.0f });
			itemEmitter_->ItemAdd(count, 30, { 0.0f, -1.2f, 0.0f }, ObjFactory::GetInstance()->GetModel("itemEffect"));
			defense.count++;
		}
	}
	else
	{
		defense.timer = 0;
		defense.count = 0;
	}

	ItemManager::GetInstance()->BuffUpdate();
}

void Hunter::SetAttackFlag(bool isAttackFlag)
{
	isAttackFlag_ = isAttackFlag;
	attackCoolTimer_ = 0;
}

Sphere Hunter::GetAttackHit()
{
	//�U���͈�
	XMVECTOR v0 = { 0, 0, 1, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(hunter_[animationType_]->GetRotation().x));
	rotM *= XMMatrixRotationY(XMConvertToRadians(hunter_[animationType_]->GetRotation().y));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR bossTarget = { hunter_[animationType_]->GetPosition().x, hunter_[animationType_]->GetPosition().y, hunter_[animationType_]->GetPosition().z };
	XMVECTOR v3 = bossTarget + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
	XMFLOAT3 pos = f;

	Sphere hitSphere;
	hitSphere.center = { weapon_->GetMatWorld().r[3].m128_f32[0], weapon_->GetMatWorld().r[3].m128_f32[1], weapon_->GetMatWorld().r[3].m128_f32[2], 1 };

	return hitSphere;
}

void Hunter::Halt()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.halt = true;
	animationType_ = static_cast<int>(AnimationType::HALT);
	hunter_[animationType_]->PlayAnimation();
	// ��~
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });
	animationEaseData_->SetActFlag(false);
}

void Hunter::Move()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.move = true;
	animationType_ = static_cast<int>(AnimationType::MOVE);
	hunter_[animationType_]->PlayAnimation();
	// �ړ�
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });
	animationEaseData_->SetActFlag(false);
}

void Hunter::Damage()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.damage = true;
	animationType_ = static_cast<int>(AnimationType::DAMAGE);
	hunter_[animationType_]->PlayAnimation();
	// �_���[�W
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ 0.0f,90.0f,0.0f });
	animationEaseData_->SetCount(30);
	animationEaseData_->SetActFlag(true);
}

void Hunter::Death()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.death = true;
	animationType_ = static_cast<int>(AnimationType::DEATH);
	hunter_[animationType_]->PlayAnimation(0, false);
	// ���S
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f, 0.0f, 2.3f });
	weapon_->SetRotation({ -100.0f, 0.0f, 90.0f });
	hunter_[animationType_]->SetRotation({});
	XMFLOAT3 position = hunter_[animationType_]->GetPosition();
	if (position.y <= 2.0f)
	{
		position.y = 2.0f;
	}
	hunter_[animationType_]->SetPosition(position);
	isDeath_ = true;
}

void Hunter::Dash()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.dash = true;
	animationType_ = static_cast<int>(AnimationType::DASH);
	hunter_[animationType_]->PlayAnimation();
	// �ړ�
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });
	animationEaseData_->SetActFlag(false);
}

void Hunter::Dodge()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.dodge = true;
	animationType_ = static_cast<int>(AnimationType::DODGE);
	hunter_[animationType_]->PlayAnimation();
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });
	animationEaseData_->SetCount(20);
	animationEaseData_->SetActFlag(true);
}

void Hunter::Combo1()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.attack1 = true;
	animationType_ = static_cast<int>(AnimationType::COMBO1);
	actFlag_ = true;
	hunter_[animationType_]->PlayAnimation(0, false);
	// �U��
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -90.0f,180.0f,0.0f });
	comboFlag_ = false;
	animationEaseData_->SetCount(40);
	animationEaseData_->SetActFlag(true);
}

void Hunter::Combo2()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.attack1 = true;
	falg_.attack2 = true;
	animationType_ = static_cast<int>(AnimationType::COMBO2);
	actFlag_ = true;
	hunter_[animationType_]->PlayAnimation(0, false);
	// �U��
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -90.0f,180.0f,0.0f });
	comboFlag_ = false;
	animationEaseData_->SetCount(40);
	animationEaseData_->SetActFlag(true);
}

void Hunter::Combo3()
{
	AnimationFlag temp = {};
	falg_ = temp;
	falg_.attack1 = true;
	falg_.attack2 = true;
	falg_.attack3 = true;
	animationType_ = static_cast<int>(AnimationType::COMBO3);
	actFlag_ = true;
	hunter_[animationType_]->PlayAnimation(0, false);
	// �U��
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -90.0f,180.0f,0.0f });
	comboFlag_ = false;
	animationEaseData_->SetCount(60);
	animationEaseData_->SetActFlag(true);
}

void Hunter::DamageHit()
{
	if (damageFlag_ && invincibleTimer_ >= 60 && !falg_.damage)
	{
		invincibleTimer_ = 0;
		damageFlag_ = false;
		hp_ -= damage_ / ItemManager::GetInstance()->DefenseBuffMagnification();
		damage_ = 0.0f;
		isAttackFlag_ = false;
		attackCoolTimer_ = 0;
		comboFlag_ = false;

		if (!falg_.damage)
		{
			animationFunc_[static_cast<int>(AnimationType::DAMAGE)]();
		}
	}
	else
	{
		if (falg_.damage && hunter_[2]->AnimationEnd())
		{
			falg_.damage = false;
		}
		damageFlag_ = false;
		invincibleTimer_++;
		damage_ = 0.0f;
	}
}
