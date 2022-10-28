#pragma once

#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "PostEffect.h"

class BaseGame
{
public:
	//���s
	void Run();
	// ������
	virtual void Initialize();
	//�I��
	virtual void Finalize();
	// ���t���[������
	virtual void Update();
	// �`��
	virtual void Draw();

protected:
	bool endRequst_ = false;
	std::unique_ptr<WinApp> win_;
	std::unique_ptr<PostEffect> postEffect_;
	//�V�[���t�@�N�g��
	AbstractSceneFactory* sceneFactory_ = nullptr;
};