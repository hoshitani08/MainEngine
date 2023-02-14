#include "Hunter.h"
#include "FbxFactory.h"
#include "ObjFactory.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Vector.h"
#include "Collision.h"
#include "DebugText.h"
#include "ItemManager.h"

std::unique_ptr<Hunter> Hunter::Create()
{
	// 3Dオブジェクトのインスタンスを生成
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
	// プレイヤー関係
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

	// 武器関係
	weapon_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("katana"));
	float s = 0.05f;
	weapon_->SetScale({ s,s,s });
	weapon_->SetBoneName("mixamorig:LeftHandThumb4");
	weapon_->SetParent(hunter_[animationType_].get());
	weapon_->SetPosition({ 0.0f,0.0f,2.3f });
	weapon_->SetRotation({ -60.0f,90.0f,45.0f });

	// アイテム用のパーティクル
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
		// 移動
		BaseMove();
		// 回避
		AvoidMove();
		// 攻撃
		AttackMove();
	}
	else
	{
		if (!falg_.death)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.death = true;
			animationType_ = 3;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 死亡
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
		}
		if (falg_.death)
		{
			isDeath_ = true;
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
		// スピード計算
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
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.dash = true;
			animationType_ = 4;
			hunter_[animationType_]->PlayAnimation();
			// 移動
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -60.0f,90.0f,45.0f });
			animationEaseData_->SetActFlag(false);
		}
		else if (!falg_.move && !isDash_ && !falg_.dodge)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.move = true;

			int count = animationType_;
			animationType_ = 1;
			hunter_[animationType_]->PlayAnimation();
			// 移動
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -60.0f,90.0f,45.0f });
			animationEaseData_->SetActFlag(false);
			
		}
	}
	else if (!isAttackFlag_ && !falg_.damage && !falg_.dodge)
	{
		if (!falg_.halt)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.halt = true;
			animationType_ = 0;
			hunter_[animationType_]->PlayAnimation();
			// 停止
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -60.0f,90.0f,45.0f });
			animationEaseData_->SetActFlag(false);
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

	// 回避
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10 && isStamina_ && !falg_.damage && !isAttackFlag_ && (input->PadStickGradient().x != 0.0f || input->PadStickGradient().y != 0.0f))
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;

		if (!falg_.dodge)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.dodge = true;
			animationType_ = 5;
			hunter_[animationType_]->PlayAnimation();
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -60.0f,90.0f,45.0f });
			animationEaseData_->SetCount(20);
			animationEaseData_->SetActFlag(true);
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

	// 攻撃
	if ((input->TriggerPadKey(BUTTON_Y) || input->TriggerPadKey(BUTTON_B)) && !avoidFlag_ &&  attackCoolTimer_ >= 10 && !itemSelectionFlag_ && !falg_.damage)
	{
		attackCoolTimer_ = 0;
		comboFlag_ = true;
	}

	// コンボしていたら
	if (comboFlag_)
	{
		isAttackFlag_ = true;
		attackCoolTimer_ = 0;
		if (falg_.attack1 && falg_.attack2 && !falg_.attack3 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.attack1 = true;
			falg_.attack2 = true;
			falg_.attack3 = true;
			animationType_ = 8;
			actFlag_ = true;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 攻撃
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -90.0f,180.0f,0.0f });
			comboFlag_ = false;
			animationEaseData_->SetCount(60);
			animationEaseData_->SetActFlag(true);
		}
		else if (falg_.attack1 && !falg_.attack2 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.attack1 = true;
			falg_.attack2 = true;
			animationType_ = 7;
			actFlag_ = true;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 攻撃
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -90.0f,180.0f,0.0f });
			comboFlag_ = false;
			animationEaseData_->SetCount(40);
			animationEaseData_->SetActFlag(true);
		}
		else if (!falg_.attack1 || falg_.attack1 && falg_.attack2 && falg_.attack3 && hunter_[animationType_]->AnimationEnd() && !actFlag_)
		{
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.attack1 = true;
			animationType_ = 6;
			actFlag_ = true;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 攻撃
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ -90.0f,180.0f,0.0f });
			comboFlag_ = false;
			animationEaseData_->SetCount(40);
			animationEaseData_->SetActFlag(true);
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

	// アイテムの選択
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

	// アイテムの使用
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

	// パーティクルの数
	int count = 4;
	if (ItemManager::GetInstance()->IsAttackBuff() && ItemManager::GetInstance()->IsDefenseBuff())
	{
		count = 2;
	}

	// 攻撃パーティクル
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
	// 防御パーティクル
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

void Hunter::AttackHit(bool isAttackFlag)
{
	isAttackFlag_ = isAttackFlag;
	attackCoolTimer_ = 0;
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
			AnimationFlag temp = {};
			falg_ = temp;
			falg_.damage = true;
			animationType_ = 2;
			hunter_[animationType_]->PlayAnimation();
			// ダメージ
			weapon_->SetParent(hunter_[animationType_].get());
			weapon_->SetPosition({ 0.0f,0.0f,2.3f });
			weapon_->SetRotation({ 0.0f,90.0f,0.0f });
			animationEaseData_->SetCount(30);
			animationEaseData_->SetActFlag(true);
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
