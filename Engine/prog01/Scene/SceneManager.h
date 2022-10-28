#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
private:
	SceneManager() = default;
	~SceneManager();

public:
	SceneManager(const SceneManager& sceneManager) = delete;
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	static SceneManager* GetInstance();

	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	// �G�t�F�N�g�`��
	void EffectDraw();
	//���V�[���̗\��
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;
	//�V�[���t�@�N�g��
	AbstractSceneFactory* sceneFactory_ = nullptr;
};