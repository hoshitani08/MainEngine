#include "ClearScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include "ObjFactory.h"
#include "Ease.h"

ClearScene::~ClearScene()
{
	Finalize();
}

void ClearScene::Initialize()
{
	// �J��������
	camera_ = std::make_unique<Camera>(WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera_.get());

	//���C�g����
	light_ = LightGroup::Create();
	//�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light_.get());
	light_->SetDirLightActive(0, true);
	light_->SetDirLightActive(1, false);
	light_->SetDirLightActive(2, false);
	light_->SetPointLightActive(0, false);
	light_->SetPointLightActive(1, false);
	light_->SetPointLightActive(2, false);
	light_->SetCircleShadowActive(0, true);

	// 3D�I�u�W�F�N�g����
	clearTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("clear"));
	clearTile_->SetRotation({ -90,0,0 });
	clearTile_->SetScale({ 50, 1, 16.6f });
	clearTile_->SetPosition({ 0,10,50 });

	continueTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("continue"));
	continueTile_->SetRotation({ -90,0,0 });
	continueTile_->SetScale({ 30, 1, 10 });
	continueTile_->SetPosition({ 0,-5,50 });

	quitTile_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("quit"));
	quitTile_->SetRotation({ -90,0,0 });
	quitTile_->SetScale({ 30, 1, 10 });
	quitTile_->SetPosition({ 0,-15,50 });

	//�X�^�[�g�ʒu��ݒ�
	startPosition_[0] = clearTile_->GetPosition();
	startPosition_[1] = continueTile_->GetPosition();
	startPosition_[2] = quitTile_->GetPosition();
	//�G���h�ʒu��ݒ�
	endPosition_[0] = { startPosition_[0].x, startPosition_[0].y, 0 };
	endPosition_[1] = { startPosition_[1].x, startPosition_[1].y, 0 };
	endPosition_[2] = { startPosition_[2].x, startPosition_[2].y, 0 };

	// �J���������_���Z�b�g
	camera_->SetTarget({ 0, 0, 0 });
	camera_->SetEye({ 0,1,-15 });

	sceneChange_ = std::make_unique<SceneChange>();
}

void ClearScene::Finalize()
{
}

void ClearScene::Update()
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
			else if (!determinationFlag_)
			{
				exit(1);
			}
		}
		if (input->TriggerPadKey(BUTTON_DPAD_UP) || input->TriggerPadKey(BUTTON_DPAD_DOWN) || (input->PadStickGradient().y != 0 && input->PadStickTriggerY()))
		{
			if (!determinationFlag_)
			{
				determinationFlag_ = true;
				savePos_ = { 0,-5,0 };
			}
			else if (determinationFlag_)
			{
				determinationFlag_ = false;
				savePos_ = { 0,-15,0 };
			}
			isShake_ = true;
		}
	}

	Select();
	EaseMove();

	clearTile_->Update();
	continueTile_->Update();
	quitTile_->Update();
	sceneChange_->Update();
}

void ClearScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�N�W�F�N�g�̕`��
	clearTile_->Draw(cmdList);
	continueTile_->Draw(cmdList);
	quitTile_->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	sceneChange_->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void ClearScene::EffectDraw()
{
}

void ClearScene::Select()
{
	if (!determinationFlag_)
	{
		continueTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		continueTile_->SetScale({ 10, 1, 5 });
		quitTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		quitTile_->SetScale({ 30, 1, 10 });
	}
	else if (determinationFlag_)
	{
		continueTile_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		continueTile_->SetScale({ 30, 1, 10 });
		quitTile_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		quitTile_->SetScale({ 10, 1, 5 });
	}

	if (isEaseFlag_)
	{
		Shake();
	}
}

void ClearScene::Shake()
{
	Input* input = Input::GetInstance();

	if (!determinationFlag_ && isShake_) {
		XMFLOAT3 shake = {};
		shakeTimer_++;

		input->SetVibration(true);

		if (shakeTimer_ > 0) {
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
			continueTile_->SetPosition(shake);
		}
		else if (attenuation_ >= 6)
		{
			shakeTimer_ = 0;
			attenuation_ = 0;
			isShake_ = 0;
			input->SetVibration(false);
			continueTile_->SetPosition(savePos_);
		}
	}
}

void ClearScene::EaseMove()
{
	XMFLOAT4 startColor = { 1, 1, 1, 0 };
	XMFLOAT4 endColor = { 1, 1, 1, 1 };
	float timeRate = 0.0f;

	if (!isEaseFlag_)
	{
		int countNum = 120;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		clearTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[0], endPosition_[0], timeRate));
		continueTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[1], endPosition_[1], timeRate));
		quitTile_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startPosition_[2], endPosition_[2], timeRate));

		clearTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startColor, endColor, timeRate));
		continueTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, { 1.0f, 0.5f, 0.5f, 0.0f }, { 1.0f, 0.5f, 0.5f, 1.0f }, timeRate));
		quitTile_->SetColor(Ease::Action(EaseType::Out, EaseFunctionType::Quad, startColor, endColor, timeRate));

		if (easeTimer_ > countNum)
		{
			isEaseFlag_ = true;
			easeTimer_ = 0;
		}
	}
}
