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

#include "DirectXCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "Input.h"

using namespace DirectX;

GameScene::~GameScene()
{
	Finalize();
}

void GameScene::Initialize()
{
	collisionManager = CollisionManager::GetInstance();

	// カメラ生成
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera.get());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera.get());

	// 背景スプライト生成
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// パーティクルマネージャ生成
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//ライト生成
	light = LightGroup::Create();
	//オブジェクトにライトをセット
	Object3d::SetLight(light.get());
	FbxObject3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	// 3Dオブジェクト生成

	// FBXオブジェクト生成
	fbxObject3d = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("uma"), L"BasicFBX", true);
	//アニメーション
	fbxObject3d->PlayAnimation();

	// カメラ注視点をセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0,1,-15 });
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	Input* input = Input::GetInstance();
	light->Update();
	camera->Update();
	particleMan->Update();

	// オブジェクト移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		static XMVECTOR p = { 1,-1,1,0 };
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			p.m128_f32[1] += 0.1f;
		}
		else if (input->PushKey(DIK_S))
		{
			p.m128_f32[1] -= 0.1f;
		}

		if (input->PushKey(DIK_D))
		{
			p.m128_f32[0] += 0.1f;
		}
		else if (input->PushKey(DIK_A))
		{
			p.m128_f32[0] -= 0.1f;
		}
		light->SetDirLightDir(0, p);
	}

	XMFLOAT3 rot = fbxObject3d->GetRotation();
	rot.y += 1.0f;
	fbxObject3d->SetRotation(rot);

	if (input->TriggerKey(DIK_C))
	{
		SceneManager::GetInstance()->ChangeScene("ClearScene");
	}
	else if (input->TriggerKey(DIK_B))
	{
		SceneManager::GetInstance()->ChangeScene("GameOverScene");
	}

	fbxObject3d->Update();
	// 全ての衝突をチェック
	collisionManager->CheckAllCollisions();
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
	fbxObject3d->Draw(cmdList);
#pragma endregion 3Dオブジェクト(FBX)描画
#pragma region パーティクル
	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion パーティクル
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}