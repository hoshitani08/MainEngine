#include "GameCamera.h"
#include "WinApp.h"
#include "Input.h"

GameCamera::GameCamera()
{
	// 初期化
	Initialize();
}

GameCamera::~GameCamera()
{
}

void GameCamera::Initialize()
{
	// カメラ生成
	camera_ = std::make_unique<Camera>(WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT);
	// イージングデータの初期化
	easeCamera = std::make_unique<EaseData>(0);
}

void GameCamera::CameraAngle(XMFLOAT2 angle)
{
	//半径は-10
	XMVECTOR v0 = { 0, 0, -10, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angle.y));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angle.x));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR target = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
	XMVECTOR v3 = target + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { target.m128_f32[0], target.m128_f32[1], target.m128_f32[2] };
	XMFLOAT3 pos = f;

	camera_->SetTarget(center);
	camera_->SetEye(pos);
	camera_->Update();
}

void GameCamera::StratCameraMove()
{
	float timeRate = 0.0f;

	easeCamera->SetActFlag(true);
	easeCamera->SetCount(120);

	XMVECTOR v0 = { 0, 0, Ease::Action(EaseType::Out, EaseFunctionType::Quad, -3.0f, -10.0f, easeCamera->GetTimeRate()), 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, 30.0f, 0.0f, easeCamera->GetTimeRate())));
	rotM *= XMMatrixRotationY(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, -130.0f, 0.0f, easeCamera->GetTimeRate())));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
	XMVECTOR v3 = bossTarget + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
	XMFLOAT3 pos = f;

	camera_->SetTarget(center);
	camera_->SetEye(pos);
	camera_->Update();
	easeCamera->Update();
}

void GameCamera::GamePlayCameraMove()
{
	Input* input = Input::GetInstance();

	if ((input->PadRightStickGradient().x != 0.0f || input->PadRightStickGradient().y != 0.0f) && !cameraResetFlag)
	{
		XMFLOAT2 speed = { input->PadRightStickGradient().x * 5.5f, input->PadRightStickGradient().y * 5.5f };

		if (speed.x < 0)
		{
			speed.x *= -1;
		}
		if (speed.y < 0)
		{
			speed.y *= -1;
		}

		angle_.x += input->PadRightStickGradient().x * speed.x;
		angle_.y += input->PadRightStickGradient().y * speed.y;
	}

	if (input->TriggerPadLeft() && !cameraResetFlag)
	{
		cameraResetFlag = true;
	}

	if (!cameraResetFlag)
	{
		CameraAngle(angle_);
	}
	else if (cameraResetFlag)
	{
		CameraReset();
	}

	//アングルの制限
	if (angle_.x >= RESTRICTION_ANGLE.x)
	{
		angle_.x = (angle_.x - RESTRICTION_ANGLE.x);
	}
	else if (angle_.x <= -RESTRICTION_ANGLE.x)
	{
		angle_.x = (angle_.x - -RESTRICTION_ANGLE.x);
	}

	if (angle_.y >= RESTRICTION_ANGLE.y)
	{
		angle_.y = 80.0f;
	}
	else if (angle_.y <= -RESTRICTION_ANGLE.y)
	{
		angle_.y = -80.0f;
	}
}

void GameCamera::EndCameraMove()
{
	easeCamera->SetCount(250);
	easeCamera->SetActFlag(true);

	XMVECTOR v0 = { 0, 0, Ease::Action(EaseType::Out, EaseFunctionType::Quad, -3, -20, easeCamera->GetTimeRate()), 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, 0.0f, 50, easeCamera->GetTimeRate())));
	rotM *= XMMatrixRotationY(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, -180.0f, 150.0f, easeCamera->GetTimeRate())));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
	XMVECTOR v3 = bossTarget + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
	XMFLOAT3 pos = f;

	camera_->SetTarget(center);
	camera_->SetEye(pos);
	camera_->Update();
	easeCamera->Update();
}

void GameCamera::CameraReset()
{
	XMFLOAT2 tempAngle = { hunter_->GetRotation().y, hunter_->GetRotation().x };

	easeCamera->SetActFlag(true);
	easeCamera->SetCount(10);

	CameraAngle(Ease::Action(EaseType::In, EaseFunctionType::Quad, angle_, tempAngle, easeCamera->GetTimeRate()));

	if (easeCamera->GetEndFlag())
	{
		cameraResetFlag = false;
		angle_ = tempAngle;
		easeCamera->Reset();
		easeCamera->SetActFlag(false);
	}

	easeCamera->Update();
}

void GameCamera::EaseDataReset()
{
	easeCamera->Reset();
	easeCamera->SetActFlag(false);
}
