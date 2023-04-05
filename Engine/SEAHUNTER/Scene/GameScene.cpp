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
	camera_ = std::make_unique<GameCamera>();

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_->GetCamerapoint());
	// FBXオブジェクトにカメラをセット
	FbxObject3d::SetCamera(camera_->GetCamerapoint());

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

	// FBXオブジェクト生成
	hunter_ = Hunter::Create();
	monster_ = Monster::Create(camera_->GetCamerapoint(), hunter_.get());
	//ステージ生成
	stage_ = std::make_unique<Stage>(monster_.get(), hunter_.get(), camera_->GetCamerapoint(), "Stage");
	// カメラのターゲットの設定
	camera_->SetHunter(hunter_.get());

	//UI
	ui_ = std::make_unique<UserInterface>(hunter_.get(), monster_.get());
	ui_->Initialize();
	// アイテム関係の初期化
	ItemManager::GetInstance()->Initialize();
	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();

	// 段階の構築
	func_.push_back(std::bind(&GameScene::GameStrat, this));
	func_.push_back(std::bind(&GameScene::GamePlay, this));
	func_.push_back(std::bind(&GameScene::GameOverEnd, this));
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

	// 階層分け
	func_[phase_]();

	hunter_->Update();
	monster_->Update();
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
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	stage_->Draw(cmdList);
#pragma endregion 3Dオブジェクト描画
#pragma region 3Dオブジェクト(FBX)描画
	monster_->Draw(cmdList);
	hunter_->Draw(cmdList);
#pragma endregion 3Dオブジェクト(FBX)描画
}

void GameScene::GameStrat()
{
	camera_->StratCameraMove();

	if (camera_->GetCameraMoveEnd() && sceneChange_->GetinEndFlag())
	{
		phase_ = 1;
		camera_->EaseDataReset();
	}
}

void GameScene::GameOverEnd()
{
	if (camera_->GetCameraMoveEnd())
	{
		sceneChange_->SceneChangeStart("GameOverScene");
		return;
	}

	camera_->EndCameraMove();
}

void GameScene::GamePlay()
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

	camera_->GamePlayCameraMove();

	hunter_->SetAngle(camera_->GetCameraAngle());
	hunter_->Behavior();

	if (hunter_->IsAttackFlag())
	{
		monster_->DamageHit(hunter_->GetAttackHit());
	}

	monster_->AllMove();

	if (ui_->GetIsPlayerDeath() && hunter_->GetIsDeath() || quest_.minute >= 15)
	{
		phase_ = 2;
	}
	else if (monster_->GetIsDead())
	{
		sceneChange_->SceneChangeStart("ClearScene");
	}

	ui_->ClockCalculate(quest_.minute);
}
