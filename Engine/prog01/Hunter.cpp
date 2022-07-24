#include "Hunter.h"
#include "FbxFactory.h"

#include "DirectXCommon.h"
#include "Input.h"
#include "Vector.h"

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
	hunter_->SetPosition({ 0, 10, -30 });
	//fbxObject3d->PlayAnimation(2);
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
		avoidTimer_++;

		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) * 1.2f;

		if (avoidTimer_ >= 10)
		{
			avoidTimer_ = 0;
			avoidFlag_ = false;
			//fbxObject3d->PlayAnimation(2);
		}
	}
	else
	{
		speed_ = (float)sqrt(input->PadStickGradient().x * input->PadStickGradient().x + input->PadStickGradient().y * input->PadStickGradient().y) / 2;
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

	if (input->TriggerPadKey(BUTTON_A) && !avoidFlag_ && avoidTimer_ >= 10)
	{
		avoidFlag_ = true;
		avoidTimer_ = 0;
		//fbxObject3d->PlayAnimation();
	}
}

void Hunter::Attack()
{

}