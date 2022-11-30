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
	float size = 0.006f;
	hunter_[0] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("halt"), L"BasicFBX", true);
	hunter_[1] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("move"), L"BasicFBX", true);
	hunter_[2] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("damage"), L"BasicFBX", true);
	hunter_[3] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("attack"), L"BasicFBX", true);
	hunter_[4] = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("death"), L"BasicFBX", true);

	for (int i = 0; i < hunter_.size(); i++)
	{
		hunter_[i]->SetScale({ size, size, size });
		hunter_[i]->SetRotation({ 0.0f,0.0f,0.0f });
		hunter_[i]->SetPosition({ 0.0f, 10.0f, -30.0f });
	}
	hunter_[animationType_]->PlayAnimation();

	buki_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("katana"));
	float s = 0.05f;
	buki_->SetScale({ s,s,s });
	buki_->SetBoneName("mixamorig:LeftHandThumb4");

	itemParticle_ = std::make_unique<ObjParticle>();
	itemParticle_->SetParent(hunter_[animationType_].get());
	itemEmitter_ = std::make_unique<ParticleEmitter>(itemParticle_.get());
	itemEmitter_->SetCenter({ 150.0f, 350.0f, 150.0f });
	itemEmitter_->SetVelocity(5.0f);
	float scale = 10.0f;
	itemEmitter_->SetObjStartScale({ scale, scale, scale });
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
	buki_->Update();
	itemEmitter_->Update();
}

void Hunter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	hunter_[animationType_]->Draw(cmdList);

	buki_->Draw(cmdList);
	itemEmitter_->Draw(cmdList);
}

void Hunter::Behavior()
{
	if (hp_ >= 1.0f)
	{
		// �X�s�[�h�v�Z
		SpeedCalculate();
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
			falg_.halt = false;
			falg_.move = false;
			falg_.damage = false;
			falg_.attack = false;
			falg_.death = true;
			animationType_ = 4;
			hunter_[animationType_]->PlayAnimation(0, false);
			// ���S
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0.0f, 0.0f, 2.3f });
			buki_->SetRotation({ -100.0f, 0.0f, 90.0f });
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
		XMFLOAT2 angle = { cameraAngle_.x + (float)input->PadStickAngle(), cameraAngle_.y };

		float speed = speed_;
		if (input->PadStickGradient().y < 0.0f)
		{
			speed *= -1.0f;
		}

		position.x +=  cosf((angle.x * 3.14f) / 180.0f) * speed_;
		position.y +=  sinf((angle.y * 3.14f) / 180.0f) * speed;
		position.z += -sinf((angle.x * 3.14f) / 180.0f) * speed_;

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


		if (input->PadStickGradient().y > 0.0f)
		{
			angle.y = -angle.y;
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
			// �ړ�
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0.0f,0.0f,2.3f });
			buki_->SetRotation({ -60.0f,90.0f,45.0f });
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
			// ��~
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0.0f,0.0f,2.3f });
			buki_->SetRotation({ -60.0f,90.0f,45.0f });
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

	// ���
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10 && isStamina_ && !falg_.damage)
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;
	}
}

void Hunter::AttackMove()
{
	Input* input = Input::GetInstance();

	// �U��
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
			// �U��
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0.0f,0.0f,1.7f });
			buki_->SetRotation({ 0.0f,180.0f,0.0f });
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
			stamina_ -= 10.0f;
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

	// �A�C�e���̎g�p
	if (input->TriggerPadKey(BUTTON_X) && !itemSelectionFlag_)
	{
		int count = ItemManager::GetInstance()->GetItemQuantity(itemType_);
		

		if (ItemManager::GetInstance()->GetItemType(itemType_) == ItemManager::ItemType::Healing && count > 0)
		{
			count--;
			hp_ += 10.0f;
			if (hp_ >= MAX_HP)
			{
				hp_ = MAX_HP;
			}
			itemParticle_->SetParent(hunter_[animationType_].get());
			itemParticle_->Update();
			itemEmitter_->ItemAdd(10, 60, hunter_[animationType_]->GetPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
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
			// �_���[�W
			buki_->SetParent(hunter_[animationType_].get());
			buki_->SetPosition({ 0.0f,0.0f,2.3f });
			buki_->SetRotation({ 0.0f,90.0f,0.0f });
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
