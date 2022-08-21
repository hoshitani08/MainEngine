#include "GameOverScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"
#include "Ease.h"

GameOverScene::~GameOverScene()
{
	Finalize();
}

void GameOverScene::Initialize()
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

	// 3Dオブジェクト生成
	gameoverTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("gameover"));
	gameoverTile_->SetRotation({ -90,0,0 });
	gameoverTile_->SetScale({ 50, 1, 16.6f });
	gameoverTile_->SetPosition({ 0,10,50 });

	continueTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("continue"));
	continueTile_->SetRotation({ -90,0,0 });
	continueTile_->SetScale({ 30, 1, 10 });
	continueTile_->SetPosition({ 0,-5,50 });

	quitTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("quit"));
	quitTile_->SetRotation({ -90,0,0 });
	quitTile_->SetScale({ 30, 1, 10 });
	quitTile_->SetPosition({ 0,-15,50 });

	//スタート位置を設定
	startPosition_[0] = gameoverTile_->GetPosition();
	startPosition_[1] = continueTile_->GetPosition();
	startPosition_[2] = quitTile_->GetPosition();
	//エンド位置を設定
	endPosition_[0] = { startPosition_[0].x, startPosition_[0].y, 0 };
	endPosition_[1] = { startPosition_[1].x, startPosition_[1].y, 0 };
	endPosition_[2] = { startPosition_[2].x, startPosition_[2].y, 0 };

	// カメラ注視点をセット
	camera_->SetTarget({ 0, 0, 0 });
	camera_->SetEye({ 0,1,-15 });
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
{
	Input* input = Input::GetInstance();

	if (isEaseFlag)
	{
		if (input->TriggerPadKey(BUTTON_A))
		{
			if (determinationFlag)
			{
				SceneManager::GetInstance()->ChangeScene("TitleScene");
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
				savePos = { 0,-5,0 };
			}
			else if (determinationFlag)
			{
				determinationFlag = false;
				savePos = { 0,-15,0 };
			}
			isShake = true;
		}
	}

	Select();
	EaseMove();

	gameoverTile_->Update();
	continueTile_->Update();
	quitTile_->Update();
}

void GameOverScene::Draw()
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
	gameoverTile_->Draw();
	continueTile_->Draw();
	quitTile_->Draw();
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameOverScene::EffectDraw()
{
}

void GameOverScene::Select()
{
	if (!determinationFlag)
	{
		continueTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		continueTile_->SetScale({ 10, 1, 5 });
		quitTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		quitTile_->SetScale({ 30, 1, 10 });
	}
	else if (determinationFlag)
	{
		continueTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		continueTile_->SetScale({ 30, 1, 10 });
		quitTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		quitTile_->SetScale({ 10, 1, 5 });
	}

	if (isEaseFlag)
	{
		Shake();
	}
}

void GameOverScene::Shake()
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

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			quitTile_->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			quitTile_->SetPosition(savePos);
		}
	}
	else if (determinationFlag && isShake)
	{
		XMFLOAT3 shake = {};
		shakeTimer++;
		input->SetVibration(true);

		if (shakeTimer > 0)
		{
			shake.x = (rand() % (7 - attenuation) - 3) + savePos.x;
			shake.y = (rand() % (7 - attenuation) - 3) + savePos.y;
			shake.z = savePos.z;
		}

		if (shakeTimer >= attenuation * 2)
		{
			attenuation += 1;
			continueTile_->SetPosition(shake);
		}
		else if (attenuation >= 6)
		{
			shakeTimer = 0;
			attenuation = 0;
			isShake = 0;
			input->SetVibration(false);
			continueTile_->SetPosition(savePos);
		}
	}
}

void GameOverScene::EaseMove()
{
	XMFLOAT4 startColor = { 1, 1, 1, 0 };
	XMFLOAT4 endColor = { 1, 1, 1, 1 };
	float timeRate = 0.0f;

	if (!isEaseFlag)
	{
		int countNum = 120;
		timeRate = easeTimer / countNum;
		easeTimer++;

		gameoverTile_->SetPosition(Ease::easeOut(startPosition_[0], endPosition_[0], timeRate));
		continueTile_->SetPosition(Ease::easeOut(startPosition_[1], endPosition_[1], timeRate));
		quitTile_->SetPosition(Ease::easeOut(startPosition_[2], endPosition_[2], timeRate));

		gameoverTile_->SetColor(Ease::easeOut(startColor, endColor, timeRate));
		continueTile_->SetColor(Ease::easeOut({ 1.0f, 0.5f, 0.5f, 0.0f }, { 1.0f, 0.5f, 0.5f, 1.0f }, timeRate));
		quitTile_->SetColor(Ease::easeOut(startColor, endColor, timeRate));

		if (easeTimer > countNum)
		{
			isEaseFlag = true;
			easeTimer = 0;
		}
	}
}
