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

	// �J��������
	camera_ = std::make_unique<GameCamera>();

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera_->GetCamerapoint());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera_->GetCamerapoint());

	//���C�g����
	light_ = LightGroup::Create();
	//�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light_.get());
	FbxObject3d::SetLight(light_.get());
	light_->SetDirLightActive(0, true);
	light_->SetDirLightActive(1, false);
	light_->SetDirLightActive(2, false);
	light_->SetPointLightActive(0, false);
	light_->SetPointLightActive(1, false);
	light_->SetPointLightActive(2, false);
	light_->SetCircleShadowActive(0, false);

	// FBX�I�u�W�F�N�g����
	hunter_ = Hunter::Create();
	monster_ = Monster::Create(camera_->GetCamerapoint(), hunter_.get());
	//�X�e�[�W����
	stage_ = std::make_unique<Stage>(monster_.get(), hunter_.get(), camera_->GetCamerapoint(), "Stage");
	// �J�����̃^�[�Q�b�g�̐ݒ�
	camera_->SetHunter(hunter_.get());

	//UI
	ui_ = std::make_unique<UserInterface>(hunter_.get(), monster_.get());
	ui_->Initialize();
	// �A�C�e���֌W�̏�����
	ItemManager::GetInstance()->Initialize();
	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();

	// �i�K�̍\�z
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

	// �K�w����
	func_[phase_]();

	hunter_->Update();
	monster_->Update();
	ui_->Update();
	stage_->Update();
	sceneChange_->Update();
	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager_->CheckAllCollisions();
}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�N�W�F�N�g�̕`��

#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
#pragma endregion �p�[�e�B�N��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	ui_->NearDraw();
	sceneChange_->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void GameScene::EffectDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�N�W�F�N�g�̕`��
	stage_->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	monster_->Draw(cmdList);
	hunter_->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
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
