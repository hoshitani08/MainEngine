#include "GameScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "ContactableObject.h"
#include "SceneManager.h"
#include "FbxFactory.h"
#include "ObjFactory.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "Input.h"
#include "ItemManager.h"
#include "BaseCalculate.h"

#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::~GameScene()
{
	Finalize();
}

void GameScene::Initialize()
{
	collisionManager_ = CollisionManager::GetInstance();

	// カメラ生成
	camera_ = std::make_unique<Camera>(WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera_.get());

	// パーティクルマネージャ生成
	particleMan_ = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera_.get());

	//ライト生成
	light_ = LightGroup::Create();
	//オブジェクトにライトをセット
	Object3d::SetLight(light_.get());
	FbxObject3d::SetLight(light_.get());
	light_->SetDirLightActive(0, true);
	light_->SetDirLightActive(1, false);
	light_->SetDirLightActive(2, false);
	light_->SetPointLightActive(0, false);
	light_->SetPointLightActive(1, false);
	light_->SetPointLightActive(2, false);
	light_->SetCircleShadowActive(0, false);

	// 3Dオブジェクト生成
	hitSphere_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));

	stage_ = std::make_unique<Stage>();

	// FBXオブジェクト生成
	hunter_ = Hunter::Create();
	monster_ = Monster::Create(camera_.get(), hunter_.get());

	//UI
	ui_ = std::make_unique<UserInterface>(hunter_.get(), monster_.get());
	ui_->Initialize();

	ItemManager::GetInstance()->Initialize();

	sceneChange_ = std::make_unique<SceneChange>();

	easeCamera = std::make_unique<EaseData>(0);
}

void GameScene::Finalize()
{
	ItemManager::GetInstance()->Finalize();
}

void GameScene::Update()
{
	Input* input = Input::GetInstance();
	input->SetVibration(false);
	light_->Update();
	particleMan_->Update();

	if (!stratFlag_)
	{
		StratCameraMove();
	}
	else if (sceneChange_->GetinEndFlag())
	{
		quest_.timer++;
		if (quest_.timer >= 60)
		{
			quest_.second++;
			quest_.timer = 0;
		}
		if (quest_.second >= 60)
		{
			quest_.minute++;
			quest_.second = 0;
		}

		CameraMove();

		hunter_->SetAngle(angle_);
		hunter_->Behavior();
		PlayerAttack();

		monster_->AllMove();
	}

	if (monster_->GetIsDead())
	{
		sceneChange_->SceneChangeStart("ClearScene");
	}
	EndCameraMove();

	ui_->ClockCalculate(quest_.minute);

	hunter_->Update();
	monster_->Update();
	hitSphere_->Update();
	ui_->Update();
	stage_->Update();
	sceneChange_->Update();
	easeCamera->Update();
	// 全ての衝突をチェック
	collisionManager_->CheckAllCollisions();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画

#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画

#pragma endregion 3Dオブジェクト(FBX)描画
#pragma region パーティクル
	// パーティクルの描画
#pragma endregion パーティクル
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	ui_->NearDraw();
	sceneChange_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameScene::EffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	stage_->Draw(cmdList);
#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画
	monster_->Draw(cmdList);
	hunter_->Draw(cmdList);
#pragma endregion 3Dオブジェクト(FBX)描画
#pragma region パーティクル
	// パーティクルの描画
	particleMan_->Draw(cmdList);
#pragma endregion パーティクル
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameScene::CameraMove()
{
	Input* input = Input::GetInstance();

	if ((input->PadRightStickGradient().x != 0.0f || input->PadRightStickGradient().y != 0.0f) &&!cameraResetFlag)
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
	else if (!endFlag_ && !cameraResetFlag)
	{
		CameraAngle(angle_);
	}

	if (cameraResetFlag)
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

void GameScene::CameraAngle(XMFLOAT2 angle)
{
	//半径は-10
	XMVECTOR v0 = { 0, 0, -10, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angle.y));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angle.x));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
	XMVECTOR v3 = bossTarget + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
	XMFLOAT3 pos = f;

	camera_->SetTarget(center);
	camera_->SetEye(pos);
	camera_->Update();
}

void GameScene::CameraReset()
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
}

void GameScene::PlayerAttack()
{
	if (hunter_->IsAttackFlag())
	{
		//攻撃範囲
		XMVECTOR v0 = { 0, 0, 1, 0 };
		XMMATRIX  rotM = XMMatrixIdentity();
		rotM *= XMMatrixRotationX(XMConvertToRadians(hunter_->GetRotation().x));
		rotM *= XMMatrixRotationY(XMConvertToRadians(hunter_->GetRotation().y));
		XMVECTOR v = XMVector3TransformNormal(v0, rotM);
		XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
		XMVECTOR v3 = bossTarget + v;
		XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
		XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
		XMFLOAT3 pos = f;

		Sphere hitSphere;
		hitSphere.center = { hunter_->GetWeaponPosition().x, hunter_->GetWeaponPosition().y, hunter_->GetWeaponPosition().z, 1};

		monster_->DamageHit(hitSphere);
		hitSphere_->SetPosition(hunter_->GetWeaponPosition());
	}
}

void GameScene::StratCameraMove()
{
	float timeRate = 0.0f;

	if (!stratFlag_)
	{
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

		if (easeCamera->GetEndFlag())
		{
			stratFlag_ = true;
			easeCamera->Reset();
			easeCamera->SetActFlag(false);
		}
	}
}

void GameScene::EndCameraMove()
{
	if ((ui_->GetIsPlayerDeath() && hunter_->GetIsDeath() || quest_.minute >= 15) && !endFlag_)
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

		if (easeCamera->GetEndFlag())
		{
			endFlag_ = true;
			easeCamera->Reset();
			easeCamera->SetActFlag(false);
		}

		if (endFlag_)
		{
			sceneChange_->SceneChangeStart("GameOverScene");
		}
	}
}
