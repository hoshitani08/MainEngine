#include "BaseGame.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "Object3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ShaderManager.h"

void BaseGame::Run()
{
	Initialize();

	while (true)  // �Q�[�����[�v
	{
		Update();

		if (endRequst_)
		{
			break;
		}

		Draw();
	}

	Finalize();
}

void BaseGame::Initialize()
{
	// �Q�[���E�B���h�E�̍쐬
	win_ = std::make_unique<WinApp>();
	win_->CreateGameWindow();

	//DirectX����������
	DirectXCommon::GetInstance()->Initialize(win_.get());

	//���͂̏�����
	//input = std::make_unique<Input>();
	Input::GetInstance()->Initialize(win_->GetInstance(), win_->GetHwnd());

	// �I�[�f�B�I�̏�����
	if (!Audio::GetInstance()->Initialize())
	{
		assert(0);
		return;
	}
	// �X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT))
	{
		assert(0);
		return;
	}

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	//���C�g�ÓI������
	LightGroup::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	// FBX�I�u�W�F�N�g�ÓI������
	FbxObject3d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());
	//FBX�I�u�W�F�N�g������
	FbxLoader::GetInstance()->Initialize(DirectXCommon::GetInstance()->GetDevice());
	//�V�F�[�_�}�l�[�W���[������
	ShaderManager::GetInstance()->Initialize(DirectXCommon::GetInstance()->GetDevice());

	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(DirectXCommon::GetInstance()->GetDevice());
}

void BaseGame::Finalize()
{
	Sprite::StaticFinalize();
	Object3d::StaticFinalize();
	FbxObject3d::StaticFinalize();
	FbxLoader::GetInstance()->Finalize();
	ShaderManager::GetInstance()->Finalize();
	// �Q�[���E�B���h�E�̔j��
	win_->TerminateGameWindow();
}

void BaseGame::Update()
{
	// ���b�Z�[�W����
	if (win_->ProcessMessage())
	{
		endRequst_ = true;
		return;
	}

	// ���͊֘A�̖��t���[������
	Input::GetInstance()->Update();

	// �Q�[���V�[���̖��t���[������
	SceneManager::GetInstance()->Update();
}

void BaseGame::Draw()
{
	postEffect_->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());

	// �Q�[���V�[���̕`��
	SceneManager::GetInstance()->EffectDraw();

	postEffect_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());

	// �`��J�n
	DirectXCommon::GetInstance()->PreDraw();

	postEffect_->Draw(DirectXCommon::GetInstance()->GetCommandList());
	SceneManager::GetInstance()->Draw();
	// �`��I��
	DirectXCommon::GetInstance()->PostDraw();
}