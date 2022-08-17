#include "TitleScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	// カメラ生成
	camera_ = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera_.get());

	//ライト生成
	light_ = LightGroup::Create();
	//オブジェクトにライトをセット
	Object3d::SetLight(light_.get());
	light_->SetDirLightActive(0, true);
	light_->SetDirLightActive(1, false);
	light_->SetDirLightActive(2, false);
	light_->SetPointLightActive(0, false);
	light_->SetPointLightActive(1, false);
	light_->SetPointLightActive(2, false);
	light_->SetCircleShadowActive(0, true);

	// パーティクルマネージャ生成
	particleMan_ = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera_.get());
	bubble_ = ParticleEmitter::Create();
	bubble_->SetParticleManager(particleMan_.get());
	bubble_->SetCenter(100);

	// 3Dオブジェクト生成
	titleTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("title"));
	titleTile_->SetRotation({ -90,-20,0 });
	titleTile_->SetScale({ 50, 1, 16.6f });
	titleTile_->SetPosition({-10,10,0});

	startTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("start"));
	startTile_->SetRotation({ -90,20,0 });
	startTile_->SetScale({ 30, 1, 10 });
	startTile_->SetPosition({ 30,-5,0 });

	escapeTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("escape"));
	escapeTile_->SetRotation({ -90,20,0 });
	escapeTile_->SetScale({ 30, 1, 10 });
	escapeTile_->SetPosition({ 30,-20,0 });

	// カメラ注視点をセット
	camera_->SetTarget({ 0, 0, 0 });
	camera_->SetEye({ 0,1,-15 });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerPadKey(BUTTON_A))
	{
		if (determinationFlag)
		{
			SceneManager::GetInstance()->ChangeScene("GameScene");
		}
		else if (!determinationFlag)
		{
			exit(1);
		}
	}

	if (input->TriggerPadKey(BUTTON_DPAD_UP) || input->TriggerPadKey(BUTTON_DPAD_DOWN) || input->PadStickGradient().y != 0)
	{
		if (!determinationFlag)
		{
			determinationFlag = true;
			savePos = { 30,-5,0 };
		}
		else if (determinationFlag)
		{
			determinationFlag = false;
			savePos = { 30,-20,0 };
		}
		isShake = true;
	}
	bubbleTimer_++;
	if (bubbleTimer_ >= 20)
	{
		bubble_->BubbleAdd({ 0,-50,-10 });
		bubbleTimer_ = 0;
	}

	Select();

	titleTile_->Update();
	startTile_->Update();
	escapeTile_->Update();
	bubble_->Update();
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブクジェクトの描画
	Object3d::PreDraw(cmdList);
	titleTile_->Draw();
	startTile_->Draw();
	escapeTile_->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
	bubble_->Draw(cmdList);
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void TitleScene::EffectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

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
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void TitleScene::Select()
{
	if (!determinationFlag) {
		startTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		startTile_->SetScale({ 10, 1, 5 });
		escapeTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		escapeTile_->SetScale({ 30, 1, 10 });
	}
	else if (determinationFlag) {
		startTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		startTile_->SetScale({ 30, 1, 10 });
		escapeTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		escapeTile_->SetScale({ 10, 1, 5 });
	}

	Shake();
}

void TitleScene::Shake()
{
	Input* input = Input::GetInstance();

	if (!determinationFlag && isShake) {
		XMFLOAT3 shake = {};
		shakeTimer++;

		input->SetVibration(true);

		if (shakeTimer > 0) {
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2) {
			attenuation += 1;
			escapeTile_->SetPosition(shake);
		}
		else if (attenuation >= 6) {
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			escapeTile_->SetPosition(savePos);
		}
	}
	else if (determinationFlag && isShake) {
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0) {
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2) {
			attenuation += 1;
			startTile_->SetPosition(shake);
		}
		else if (attenuation >= 6) {
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			startTile_->SetPosition(savePos);
		}
	}
}
