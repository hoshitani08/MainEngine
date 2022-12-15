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
	camera_ = std::make_unique<Camera>(WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera_.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera_.get());

	// �p�[�e�B�N���}�l�[�W������
	particleMan_ = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera_.get());

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

	// 3D�I�u�W�F�N�g����
	hitSphere_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));

	stage_ = std::make_unique<Stage>();

	// FBX�I�u�W�F�N�g����
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

	XMFLOAT3 vector = { monster_->GetPosition().x - hunter_->GetPosition().x, monster_->GetPosition().y - hunter_->GetPosition().y, monster_->GetPosition().z - hunter_->GetPosition().z };
	XMFLOAT3 enemyRot = {};

	enemyRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (180.0f / 3.14159265359f);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(-vector.y - 0.0f, vector.x - 0.0f) * (180.0f / 3.14159265359f);

	DebugText::GetInstance()->VariablePrint(0, 80, "enemyRot.y", enemyRot.y, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 96, "enemyRot.z", enemyRot.z, 1.0f);


	DebugText::GetInstance()->VariablePrint(0, 0, "angle.x", angle_.x, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 16, "angle.y", angle_.y, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 32, "playerAngle.x", hunter_->GetRotation().x, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 48, "playerAngle.y", hunter_->GetRotation().y, 1.0f);
	DebugText::GetInstance()->VariablePrint(0, 64, "playerAngle.z", hunter_->GetRotation().z, 1.0f);

	hunter_->Update();
	monster_->Update();
	hitSphere_->Update();
	ui_->Update();
	stage_->Update();
	sceneChange_->Update();
	easeCamera->Update();
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
	//ui_->NearDraw();
	sceneChange_->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void GameScene::EffectDraw()
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
	stage_->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	monster_->Draw(cmdList);
	hunter_->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
	particleMan_->Draw(cmdList);
#pragma endregion �p�[�e�B�N��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void GameScene::CameraMove()
{
	Input* input = Input::GetInstance();

	if ((input->PadRightStickGradient().x != 0.0f || input->PadRightStickGradient().y != 0.0f) && !LockOnFlag &&!cameraResetFlag)
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

	if (input->TriggerPadKey(BUTTON_RIGHT_THUMB))
	{
		/*if (!LockOnFlag)
		{
			LockOnFlag = true;
		}
		else
		{
			LockOnFlag = false;
		}*/
	}
	else if (input->TriggerPadLeft() && !cameraResetFlag)
	{
		cameraResetFlag = true;

	}
	else if (!endFlag_ && !cameraResetFlag)
	{
		CameraAngle(angle_);
	}

	if (LockOnFlag)
	{
		CameraLockOn();
	}
	else if (cameraResetFlag)
	{
		CameraReset();
	}

	int count = 0;
	if (angle_.x >= RESTRICTION_ANGLE.x)
	{
		count = (int)angle_.x % (int)RESTRICTION_ANGLE.x;
		angle_.x = (angle_.x - RESTRICTION_ANGLE.x);
	}
	else if (angle_.x <= -RESTRICTION_ANGLE.x)
	{
		count = (int)angle_.x % (int)RESTRICTION_ANGLE.x;
		angle_.x = (angle_.x - -RESTRICTION_ANGLE.x) + (float)count;
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
	//���a��-10
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

void GameScene::CameraLockOn()
{
	XMFLOAT3 vector = { monster_->GetPosition().x - hunter_->GetPosition().x, monster_->GetPosition().y - hunter_->GetPosition().y, monster_->GetPosition().z - hunter_->GetPosition().z };
	XMFLOAT3 enemyRot = {};

	enemyRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (180.0f / 3.14159265359f);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(-vector.y - 0.0f, vector.x - 0.0f) * (180.0f / 3.14159265359f);

	XMFLOAT2 tempAngle = { 1.0f, 1.0f };

	if((enemyRot.y < 0.0f && enemyRot.y > -90.0f || enemyRot.y > 0.0f && enemyRot.y < 90.0f))
	{
		tempAngle.y = 1.0f;
	}
	else if ((enemyRot.y < -90.0f && enemyRot.y > -180.0f || enemyRot.y > 90.0f && enemyRot.y < 180.0f))
	{
		tempAngle.y = -1.0f;
	}

	CameraAngle({ enemyRot.y + (90 * tempAngle.y), enemyRot.z });
}

void GameScene::PlayerAttack()
{
	if (hunter_->IsAttackFlag())
	{
		//�U���͈�
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
