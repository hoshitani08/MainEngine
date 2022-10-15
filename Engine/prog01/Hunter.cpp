#include "Hunter.h"
#include "FbxFactory.h"
#include "ObjFactory.h"

#include "DirectXCommon.h"
#include "Input.h"
#include "Vector.h"
#include "Collision.h"

#include "DebugText.h"

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
	hunter_ = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("player"), L"BasicFBX");
	hunter_->SetScale({ size, size, size });
	hunter_->SetRotation({0,0,0});
	hunter_->SetPosition({ 0, 10, -30 });
	//hunter_->PlayAnimation();
}

void Hunter::Finalize()
{
}

void Hunter::Update()
{
	DamageHit();
	hunter_->Update();
}

void Hunter::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	
	hunter_->Draw(cmdList);
}

void Hunter::Behavior()
{
	Input* input = Input::GetInstance();

	// �X�s�[�h�v�Z
	SpeedCalculate();
	// �ړ�
	BaseMove();
	// ���
	AvoidMove();
	// �U��
	AttackMove();
}

void Hunter::BaseMove()
{
	Input* input = Input::GetInstance();

	XMFLOAT3 position = hunter_->GetPosition();
	XMFLOAT3 rotation = hunter_->GetRotation();

	if (input->PadStickGradient().x != 0.0f || input->PadStickGradient().y != 0.0f)
	{
		float a = cameraAngle_.x + input->PadStickAngle();
		XMFLOAT2 angle = { a, cameraAngle_.y };

		position.x +=  cosf((angle.x * 3.14) / 180.0f) * speed_;
		position.y += -sinf((angle.y * 3.14) / 180.0f) * speed_;
		position.z += -sinf((angle.x * 3.14) / 180.0f) * speed_;

		rotation.y = angle.x + 90;
		rotation.x = angle.y;
	}
	hunter_->SetPosition(position);
	hunter_->SetRotation(rotation);
}

void Hunter::AvoidMove()
{
	Input* input = Input::GetInstance();

	// ���
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10 && isStamina_)
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;
	}
}

void Hunter::AttackMove()
{
	Input* input = Input::GetInstance();

	// �U��
	if ((input->TriggerPadKey(BUTTON_Y) || input->TriggerPadKey(BUTTON_B)) && !avoidFlag_ && !isAttackFlag_ && attackCoolTimer_ >= 10)
	{
		isAttackFlag_ = true;
		attackCoolTimer_ = 0;
	}

	if (isAttackFlag_)
	{
		if (attackCoolTimer_ >= 10)
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

void Hunter::AttackHit(bool isAttackFlag)
{
	isAttackFlag_ = isAttackFlag;
	attackCoolTimer_ = 0;
}

void Hunter::DamageHit()
{
	if (damageFlag_ && invincibleTimer_ >= 60)
	{
		invincibleTimer_ = 0;
		damageFlag_ = false;
		hp_ -= damage_;
		damage_ = 0.0f;
	}
	else
	{
		damageFlag_ = false;
		invincibleTimer_++;
		damage_ = 0.0f;
	}
}
