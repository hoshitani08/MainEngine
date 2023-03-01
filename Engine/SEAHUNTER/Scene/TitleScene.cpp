#include "TitleScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"
#include "Ease.h"
#include "BaseCalculate.h"

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	// カメラ生成
	camera_ = std::make_unique<Camera>(WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT);
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
	XMFLOAT4 startColor = { 1, 1, 1, 0 };

	titleTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("title"));
	titleTile_->SetRotation({ -90,-20,0 });
	titleTile_->SetScale({ 50, 1, 16.6f });
	titleTile_->SetPosition({-10,10,50});
	titleTile_->SetColor(startColor);

	startTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("start"));
	startTile_->SetRotation({ -90,20,0 });
	startTile_->SetScale({ 30, 1, 10 });
	startTile_->SetPosition({ 30,-5,50 });
	startTile_->SetColor({ 1.0f, 0.5f, 0.5f, 0.0f });

	quitTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("quit"));
	quitTile_->SetRotation({ -90,20,0 });
	quitTile_->SetScale({ 30, 1, 10 });
	quitTile_->SetPosition({ 30,-15,50 });

	//スタート位置を設定
	startPosition_[0] = titleTile_->GetPosition();
	startPosition_[1] = startTile_->GetPosition();
	startPosition_[2] = quitTile_->GetPosition();
	//エンド位置を設定
	endPosition_[0] = { startPosition_[0].x, startPosition_[0].y, 0 };
	endPosition_[1] = { startPosition_[1].x, startPosition_[1].y, 0 };
	endPosition_[2] = { startPosition_[2].x, startPosition_[2].y, 0 };

	ground_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("ground"));
	ground_->SetScale({ 2,2,2 });
	ground_->SetPosition({ 0,-4,0 });

	for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxX("map"); i++)
	{
		for (int j = 0; j < MapChip::GetInstance()->GetMapChipMaxY("map"); j++)
		{
			std::unique_ptr<Block> block;

			XMFLOAT2 size = { static_cast<float>(MapChip::GetInstance()->GetMapChipMaxX("map") / 2), static_cast<float>(MapChip::GetInstance()->GetMapChipMaxY("map") / 2) };
			float count = 2.5f;
			XMFLOAT3 pos = { (i - size.x) * count, -4, (j - size.y) * count };

			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 1)
			{
				block = std::make_unique<Block>(0, pos);

				block_.push_back(std::move(block));
			}
			if (MapChip::GetInstance()->GetChipNum(i, j, "map") == 2)
			{
				block = std::make_unique<Block>(1, pos);

				block_.push_back(std::move(block));
			}
		}
	}

	// カメラ注視点をセット
	camera_->SetTarget({ 0, 0, 0 });
	camera_->SetEye({ 0,1,-15 });

	sceneChange_ = std::make_unique<SceneChange>();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (isEaseFlag_ && sceneChange_->GetinEndFlag())
	{
		if (input->TriggerPadKey(BUTTON_A))
		{
			if (determinationFlag_)
			{
				sceneChange_->SceneChangeStart("GameScene");
			}
			else
			{
				exit(1);
			}
		}
		if (input->TriggerPadKey(BUTTON_DPAD_UP) || input->TriggerPadKey(BUTTON_DPAD_DOWN) || (input->PadStickGradient().y != 0 && input->PadStickTriggerY()))
		{
			if (!determinationFlag_)
			{
				determinationFlag_ = true;
				savePos_ = { 30,-5,0 };
			}
			else
			{
				determinationFlag_ = false;
				savePos_ = { 30,-15,0 };
			}
			isShake_ = true;
		}
	}

	Select();
	EaseMove();

	titleTile_->Update();
	startTile_->Update();
	quitTile_->Update();
	ground_->Update();
	for (auto& a : block_)
	{
		a->Update();
	}
	sceneChange_->Update();
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
	titleTile_->Draw(cmdList);
	startTile_->Draw(cmdList);
	quitTile_->Draw(cmdList);
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	sceneChange_->Draw();
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
	for (auto& a : block_)
	{
		a->Draw(cmdList);
	}
	ground_->Draw(cmdList);
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
	if (!determinationFlag_)
	{
		startTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		startTile_->SetScale({ 10, 1, 5 });
		quitTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		quitTile_->SetScale({ 30, 1, 10 });
	}
	else
	{
		startTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		startTile_->SetScale({ 30, 1, 10 });
		quitTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		quitTile_->SetScale({ 10, 1, 5 });
	}

	if (isEaseFlag_)
	{
		Shake();
	}
}

void TitleScene::Shake()
{
	Input* input = Input::GetInstance();

	if (!determinationFlag_ && isShake_)
	{
		XMFLOAT3 shake = {};
		shakeTimer_++;

		input->SetVibration(true);

		if (shakeTimer_ > 0)
		{
			shake.x = (rand() % (7 - attenuation_) - 3) + savePos_.x;
			shake.y = (rand() % (7 - attenuation_) - 3) + savePos_.y;
			shake.z = savePos_.z;
		}

		if (shakeTimer_ >= attenuation_ * 2)
		{
			attenuation_ += 1;
			quitTile_->SetPosition(shake);
		}
		else if (attenuation_ >= 6)
		{
			shakeTimer_ = 0;
			attenuation_ = 0;
			isShake_ = 0;
			input->SetVibration(false);
			quitTile_->SetPosition(savePos_);
		}
	}
	else if (determinationFlag_ && isShake_)
	{
		XMFLOAT3 shake = {};
		shakeTimer_++;
		input->SetVibration(true);

		if (shakeTimer_ > 0)
		{
			shake.x = (rand() % (7 - attenuation_) - 3) + savePos_.x;
			shake.y = (rand() % (7 - attenuation_) - 3) + savePos_.y;
			shake.z = savePos_.z;
		}

		if (shakeTimer_ >= attenuation_ * 2)
		{
			attenuation_ += 1;
			startTile_->SetPosition(shake);
		}
		else if (attenuation_ >= 6)
		{
			shakeTimer_ = 0;
			attenuation_ = 0;
			isShake_ = 0;
			input->SetVibration(false);
			startTile_->SetPosition(savePos_);
		}
	}
}

void TitleScene::EaseMove()
{
	XMFLOAT4 startColor = { 1, 1, 1, 0 };
	XMFLOAT4 endColor = { 1, 1, 1, 1 };
	float timeRate = 0.0f;

	if (!isEaseFlag_)
	{
		int countNum = 120;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		titleTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[0], endPosition_[0], timeRate));
		startTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[1], endPosition_[1], timeRate));
		quitTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[2], endPosition_[2], timeRate));

		titleTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startColor, endColor, timeRate));
		startTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, { 1.0f, 0.5f, 0.5f, 0.0f }, { 1.0f, 0.5f, 0.5f, 1.0f }, timeRate));
		quitTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startColor, endColor, timeRate));

		if (easeTimer_ > countNum)
		{
			isEaseFlag_ = true;
			easeTimer_ = 0;
		}
	}
}
