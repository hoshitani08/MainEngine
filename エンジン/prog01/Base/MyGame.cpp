#include "MyGame.h"
#include "MapChip.h"
#include "SceneFactory.h"
#include "SpriteFactory.h"
#include "FbxFactory.h"
#include "ObjFactory.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	BaseGame::Initialize();

	//�Q�[���ŕK�v�ȃ}�b�v�f�[�^�̃��[�h
	//MapChip::GetInstance()->CsvLoad();
	//�Q�[���ŕK�v�ȉ摜�̃��[�h
	SpriteFactory::GetInstance()->Initialize();
	//�Q�[���ŕK�v�ȃ��f��(.fbx)�̃��[�h
	FbxFactory::GetInstance()->Initialize();
	//�Q�[���ŕK�v�ȃ��f��(.obj)�̃��[�h
	ObjFactory::GetInstance()->Initialize();
	//�Q�[���ŕK�v�ȃT�E���h�̃��[�h
	Audio::GetInstance()->LoadWave(0, "Resources/Alarm01.wav");

	// �V�[���̏�����
	//�V�[���t�@�N�g���𐶐����A�}�l�[�W���[�ɃZ�b�g
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("GameScene");
}

void MyGame::Finalize()
{
	// �V�[���̏I��
	SceneManager::GetInstance()->Finalize();
	//�eFactory�̉��
	FbxFactory::GetInstance()->Finalize();
	ObjFactory::GetInstance()->Finalize();
	//���N���X�̉��
	BaseGame::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V
	BaseGame::Update();
}

void MyGame::Draw()
{
	BaseGame::Draw();
}