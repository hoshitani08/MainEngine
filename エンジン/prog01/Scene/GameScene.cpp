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

	// �J��������
	camera = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera.get());
	// FBX�I�u�W�F�N�g�ɃJ�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	// �w�i�X�v���C�g����
	sprite = Sprite::Create(1, { 0.0f,0.0f });
	sprite->SetSize({ 100.0f,100.0f });
	sprite->SetPosition({ 100.0f,100.0f });

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera.get());

	//���C�g����
	light = LightGroup::Create();
	//�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light.get());
	FbxObject3d::SetLight(light.get());
	light->SetDirLightActive(0, true);
	light->SetDirLightActive(1, false);
	light->SetDirLightActive(2, false);
	light->SetPointLightActive(0, false);
	light->SetPointLightActive(1, false);
	light->SetPointLightActive(2, false);
	light->SetCircleShadowActive(0, true);

	// 3D�I�u�W�F�N�g����

	// FBX�I�u�W�F�N�g����
	fbxObject3d = FbxObject3d::Create(FbxFactory::GetInstance()->GetModel("uma"), L"BasicFBX", true);
	//�A�j���[�V����
	fbxObject3d->PlayAnimation();

	// �J���������_���Z�b�g
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

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		static XMVECTOR p = { 1,-1,1,0 };
		// �ړ���̍��W���v�Z
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
	// �S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
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
	Object3d::PreDraw(cmdList);
	
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��
#pragma region 3D�I�u�W�F�N�g(FBX)�`��
	fbxObject3d->Draw(cmdList);
#pragma endregion 3D�I�u�W�F�N�g(FBX)�`��
#pragma region �p�[�e�B�N��
	// �p�[�e�B�N���̕`��
	particleMan->Draw(cmdList);
#pragma endregion �p�[�e�B�N��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}