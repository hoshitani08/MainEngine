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
	/// <summary>
	/// ���s
	/// </summary>
	void Run();
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

protected:
	bool endRequst_ = false;
	std::unique_ptr<WinApp> win_;
	std::unique_ptr<PostEffect> postEffect_;
	//�V�[���t�@�N�g��
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};