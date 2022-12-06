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
#include "Ease.h"
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

	//CameraMove();
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

		if (input->PadRightStickGradient().x != 0.0f || input->PadRightStickGradient().y != 0.0f)
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

			if (angle_.x >= RESTRICTION_ANGLE.x)
			{
				angle_.x = 0.0f;
			}
			else if (angle_.x <= -RESTRICTION_ANGLE.x)
			{
				angle_.x = 0.0f;
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

		hunter_->SetAngle(angle_);
		hunter_->Behavior();

		if (!endFlag_)
		{
			CameraMove();
		}
		
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
	//半径は-10
	XMVECTOR v0 = { 0, 0, -10, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angle_.y));
	rotM *= XMMatrixRotationY(XMConvertToRadians(angle_.x));
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
		hitSphere.center = { pos.x, pos.y, pos.z, 1 };

		monster_->DamageHit(hitSphere);
		hitSphere_->SetPosition(pos);
	}
}

void GameScene::StratCameraMove()
{
	float timeRate = 0.0f;

	if (!stratFlag_)
	{
		int countNum = 120;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		XMVECTOR v0 = { 0, 0, Ease::Action(EaseType::Out, EaseFunctionType::Quad, -3.0f, -10.0f, timeRate), 0 };
		XMMATRIX  rotM = XMMatrixIdentity();
		rotM *= XMMatrixRotationX(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, 30.0f, 0.0f, timeRate)));
		rotM *= XMMatrixRotationY(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, -130.0f, 0.0f, timeRate)));
		XMVECTOR v = XMVector3TransformNormal(v0, rotM);
		XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
		XMVECTOR v3 = bossTarget + v;
		XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
		XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
		XMFLOAT3 pos = f;

		camera_->SetTarget(center);
		camera_->SetEye(pos);
		camera_->Update();

		if (easeTimer_ > countNum)
		{
			stratFlag_ = true;
			easeTimer_ = 0;
		}
	}
}

void GameScene::EndCameraMove()
{
	if ((ui_->GetIsPlayerDeath() && hunter_->GetIsDeath() || quest_.minute >= 15) && !endFlag_)
	{
		float timeRate = 0.0f;

		int countNum = 200;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		XMVECTOR v0 = { 0, 0, Ease::Action(EaseType::Out, EaseFunctionType::Quad, -3, -20, timeRate), 0 };
		XMMATRIX  rotM = XMMatrixIdentity();
		rotM *= XMMatrixRotationX(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, 0.0f, 50, timeRate)));
		rotM *= XMMatrixRotationY(XMConvertToRadians(Ease::Action(EaseType::Out, EaseFunctionType::Quad, -180.0f, 150.0f, timeRate)));
		XMVECTOR v = XMVector3TransformNormal(v0, rotM);
		XMVECTOR bossTarget = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z };
		XMVECTOR v3 = bossTarget + v;
		XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
		XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
		XMFLOAT3 pos = f;

		camera_->SetTarget(center);
		camera_->SetEye(pos);
		camera_->Update();

		if (easeTimer_ > countNum)
		{
			endFlag_ = true;
			easeTimer_ = 0;
		}

		if (endFlag_)
		{
			sceneChange_->SceneChangeStart("GameOverScene");
		}
	}
}
