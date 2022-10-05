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
	hunter_ = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("player"), L"BasicFBX");
	hunter_->SetScale({ size, size, size });
	hunter_->SetRotation({0,-90,0});
	hunter_->SetPosition({ 0, 10, -30 });
}

void Hunter::Finalize()
{
}

void Hunter::Update()
{
	hunter_->Update();
}

void Hunter::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	
	hunter_->Draw(cmdList);
}

void Hunter::Move()
{
	Input* input = Input::GetInstance();

	if (avoidFlag_)
	{
		if (avoidTimer_ <= 0)
		{
			strengthDecrement_ = 10;
		}
		else
		{
			strengthDecrement_ = 0.0f;
		}

		avoidTimer_++;

		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) * 1.2f;

		if (avoidTimer_ >= 10)
		{
			avoidTimer_ = 0;
			avoidFlag_ = false;
		}
	}
	else if (input->PushPadKey(BUTTON_RIGHT_SHOULDER) && isDash_)
	{
		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y);
		strengthDecrement_ = 0.6f;
		avoidTimer_++;
	}
	else
	{
		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) / 2;
		strengthDecrement_ = 0.0f;
		avoidTimer_++;
	}

	position_ = hunter_->GetPosition();
	XMFLOAT3 rotation = hunter_->GetRotation();

	if (input->PadStickGradient().x != 0.0f || input->PadStickGradient().y != 0.0f)
	{
		float a = cameraAngle_.x + input->PadStickAngle();
		XMFLOAT2 angle = { a, cameraAngle_.y };

		position_.x +=  cosf((angle.x * 3.14) / 180.0f) * speed_;
		position_.y += -sinf((angle.y * 3.14) / 180.0f) * speed_;
		position_.z += -sinf((angle.x * 3.14) / 180.0f) * speed_;
		rotation.y = angle.x;
		rotation.z = -angle.y;
	}
	hunter_->SetPosition(position_);
	hunter_->SetRotation(rotation);

	// 回避
	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10)
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;
	}

	if (!damageFlag_)
	{
		invincibleTimer_++;
	}

	// 攻撃
	if ((input->TriggerPadKey(BUTTON_Y) || input->TriggerPadKey(BUTTON_B)) && !avoidFlag_ && avoidTimer_ >= 10 && !isAttackFlag_ && attackCoolTimer_ >= 10)
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

void Hunter::SetDamageFlag(bool damageFlag)
{
	damageFlag_ = damageFlag;

	if (invincibleTimer_ >= 60 && damageFlag)
	{
		invincibleTimer_ = 0;
	}
}