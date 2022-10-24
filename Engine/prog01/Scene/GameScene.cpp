#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
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

using namespace DirectX;

GameScene::~GameScene()
{
	Finalize();
}

void GameScene::Initialize()
{
	collisionManager_ = CollisionManager::GetInstance();

	// カメラ生成
	camera_ = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera_.get());

	// 背景スプライト生成
	sprite_ = Sprite::Create(1, { 0.0f,0.0f });
	sprite_->SetSize({ 100.0f,100.0f });
	sprite_->SetPosition({ 100.0f,100.0f });

	// パーティクルマネージャ生成
	particleMan_ = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera_.get());

	PmxObject3d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), camera_.get());

	//ライト生成
	light_ = LightGroup::Create();
	//オブジェクトにライトをセット
	Object3d::SetLight(light_.get());
	FbxObject3d::SetLight(light_.get());
	PmxObject3d::SetLight(light_.get());
	light_->SetDirLightActive(0, true);
	light_->SetDirLightActive(1, false);
	light_->SetDirLightActive(2, false);
	light_->SetPointLightActive(0, false);
	light_->SetPointLightActive(1, false);
	light_->SetPointLightActive(2, false);
	light_->SetCircleShadowActive(0, true);

	// 3Dオブジェクト生成
	skydome_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("skydome"));
	ground_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("ground"));
	hitSphere_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));

	// FBXオブジェクト生成
	hunter_ = Hunter::Create();
	monster_ = Monster::Create(camera_.get());

	monster_->SetHunter(hunter_.get());

	//UI
	ui = std::make_unique<UserInterface>();
	ui->SetHunter(hunter_.get());
	ui->SetMonster(monster_.get());
	ui->Initialize();

	ItemManager::GetInstance()->Initialize();
}

void GameScene::Finalize()
{
	ItemManager::GetInstance()->Finalize();
}

void GameScene::Update()
{
	quest.timer++;
	if (quest.timer >= 60)
	{
		quest.second++;
		quest.timer = 0;
	}
	if (quest.second >= 60)
	{
		quest.minute++;
		quest.second = 0;
	}

	Input* input = Input::GetInstance();
	input->SetVibration(false);
	light_->Update();
	particleMan_->Update();

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

	CameraMove();
	PlayerAttack();

	if (monster_->GetIsDead())
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (ui->GetIsPlayerDeath() || quest.minute >= 15)
	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}

	ui->ClockCalculate(quest.minute);

	hunter_->Update();
	monster_->Update();
	skydome_->Update();
	ground_->Update();
	hitSphere_->Update();
	ui->Update();
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
	Object3d::PreDraw(cmdList);

	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画

#pragma endregion 3Dオブジェクト(FBX)描画
#pragma region パーティクル
	// パーティクルの描画
	monster_->ParticleDraw();
#pragma endregion パーティクル
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	ui->NearDraw();
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
	Object3d::PreDraw(cmdList);
	skydome_->Draw();
	ground_->Draw();
	if (hunter_->IsAttackFlag())
	{
		hitSphere_->Draw();
	}
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画
	monster_->Draw();
	hunter_->Draw();
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
