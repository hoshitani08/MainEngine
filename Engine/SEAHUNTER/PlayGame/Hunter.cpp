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
	float size = 0.006f;
	hunter_[0] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("halt"), L"BasicFBX", true);
	hunter_[1] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("move"), L"BasicFBX", true);
	hunter_[2] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("damage"), L"BasicFBX", true);
	hunter_[3] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("attack"), L"BasicFBX", true);
	hunter_[4] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("death"), L"BasicFBX", true);

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->SetScale({ size, size, size });
		hunter_[i]->SetRotation({ 0,0,0 });
		hunter_[i]->SetPosition({ 0, 10, -30 });
	}

	buki_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("katana"));
	float s = 0.05f;
	buki_->SetScale({ s,s,s });
	// ダメージ
	//buki_->SetParent(hunter_[animationType_].get());
	//buki_->SetPosition({ 0,0,2.3f });
	//buki_->SetRotation({ 0,90,0 });
	// 死亡
	//buki_->SetPosition({ 0,0,2.3f });
	//buki_->SetRotation({ -100,0,90 });

	buki_->SetBoneName("mixamorig:LeftHandThumb4");
	

	hunter_[animationType_]->PlayAnimation();
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

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->Update();
	}
	buki_->Update();
}

void Hunter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	hunter_[animationType_]->Draw(cmdList);

	buki_->Draw(cmdList);
}

void Hunter::Behavior()
{
	if (hp_ >= 1.0f)
	{
		// スピード計算
		SpeedCalculate();
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
			falg_.halt = false;
			falg_.move = false;
			falg_.damage = false;
			falg_.attack = false;
			falg_.death = true;
			animationType_ = 4;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 死亡
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0,0,2.3f });
			buki_->SetRotation({ -100,0,90 });
		}
		if (falg_.death && hunter_[4]->AnimationEnd())
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

	if ((input->PadStickGradient().x != 0.0f || input->PadStickGradient().y != 0.0f) && !isAttackFlag_ && !falg_.damage)
	{
		float a = cameraAngle_.x + input->PadStickAngle();
		XMFLOAT2 angle = { a, cameraAngle_.y };

		position.x +=  cosf((angle.x * 3.14) / 180.0f) * speed_;
		position.y += -sinf((angle.y * 3.14) / 180.0f) * speed_;
		position.z += -sinf((angle.x * 3.14) / 180.0f) * speed_;

		if (position.x <= -48)
		{
			position.x = -48;
		}
		else if (position.x >= 48)
		{
			position.x = 48;
		}

		if (position.y <= 1)
		{
			position.y = 1;
		}
		else if (position.y >= 58)
		{
			position.y = 58;
		}

		if (position.z <= -48)
		{
			position.z = -48;
		}
		else if (position.z >= 48)
		{
			position.z = 48;
		}

		rotation.y = angle.x + 90;
		rotation.x = angle.y;

		if (!falg_.move)
		{
			falg_.halt = false;
			falg_.move = true;
			falg_.damage = false;
			falg_.attack = false;
			falg_.death = false;
			animationType_ = 1;
			hunter_[animationType_]->PlayAnimation();
			// 移動
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0,0,2.3f });
			buki_->SetRotation({ -60,90,45 });
		}
	}
	else if (!isAttackFlag_ && !falg_.damage)
	{
		if (!falg_.halt)
		{
			falg_.halt = true;
			falg_.move = false;
			falg_.damage = false;
			falg_.attack = false;
			falg_.death = false;
			animationType_ = 0;
			hunter_[animationType_]->PlayAnimation();
			// 停止
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0,0,2.3f });
			buki_->SetRotation({ -60,90,45 });
		}
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
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10 && isStamina_ && !falg_.damage)
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;
	}
}

void Hunter::AttackMove()
{
	Input* input = Input::GetInstance();

	// 攻撃
	if ((input->TriggerPadKey(BUTTON_Y) || input->TriggerPadKey(BUTTON_B)) && !avoidFlag_ && !isAttackFlag_ && attackCoolTimer_ >= 10 && !itemSelectionFlag_ && !falg_.damage)
	{
		isAttackFlag_ = true;
		attackCoolTimer_ = 0;
		if (!falg_.attack)
		{
			falg_.halt = false;
			falg_.move = false;
			falg_.damage = false;
			falg_.attack = true;
			falg_.death = false;
			animationType_ = 3;
			hunter_[animationType_]->PlayAnimation(0, false);
			// 攻撃
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0,0,1.7f });
			buki_->SetRotation({ 0,180,0 });
		}
	}

	if (isAttackFlag_)
	{
		if (attackCoolTimer_ >= 10 && hunter_[animationType_]->AnimationEnd())
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

	if (avoidFlag_)
	{
		if (avoidTimer_ <= 0)
		{
			stamina_ -= 10;
		}

		avoidTimer_++;

		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) * 1.2f;

		if (avoidTimer_ >= 10)
		{
			avoidTimer_ = 0;
			avoidFlag_ = false;
		}
	}
	else if (input->PushPadKey(BUTTON_RIGHT_SHOULDER) && isStamina_)
	{
		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y);
		stamina_ -= 0.5f;
		avoidTimer_++;
	}
	else
	{
		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) / 2;
		avoidTimer_++;
	}

	if (isStamina_ && stamina_ <= 0.0f)
	{
		stamina_ = 0.0f;
		isStamina_ = false;
	}
	else if (!isStamina_ && stamina_ >= 50)
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
		if (input->TriggerPadKey(BUTTON_X))
		{
			itemType_++;
		}
		else if (input->TriggerPadKey(BUTTON_B))
		{
			itemType_--;
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
	if (input->TriggerPadKey(BUTTON_X) && !itemSelectionFlag_)
	{
		int count = ItemManager::GetInstance()->GetItemQuantity(itemType_);
		

		if (ItemManager::GetInstance()->GetItemType(itemType_) == ItemManager::ItemType::Healing && count > 0)
		{
			count--;
			hp_ += 10;
			if (hp_ >= MAX_HP)
			{
				hp_ = MAX_HP;
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

		if (!falg_.damage)
		{
			falg_.halt = false;
			falg_.move = false;
			falg_.damage = true;
			falg_.attack = false;
			falg_.death = false;
			animationType_ = 2;
			hunter_[animationType_]->PlayAnimation();
			// ダメージ
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0,0,2.3f });
			buki_->SetRotation({ 0,90,0 });
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
