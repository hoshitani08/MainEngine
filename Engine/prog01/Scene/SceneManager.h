#pragma once

#include <memory>
#include <future>

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
public: // �T�u�N���X
	enum class LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};

private:
	SceneManager() = default;
	~SceneManager();

public:
	SceneManager(const SceneManager& sceneManager) = delete;
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	static SceneManager* GetInstance();

	// ������
	void Initialize();
	// �I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	// �G�t�F�N�g�`��
	void EffectDraw();
	// ���V�[���̗\��
	void ChangeScene(const std::string& sceneName);
	// ���[�h�V�[���̐ݒ�
	void SetLoadScene(const std::string& sceneName);
	// �V�[�������̐ݒ�
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	//���̃V�[��
	std::unique_ptr<BaseScene> scene_;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;
	//���[�h�V�[��
	std::unique_ptr<BaseScene> loadScene_;
	//�V�[���t�@�N�g��
	AbstractSceneFactory* sceneFactory_ = nullptr;
	// �񓯊�����
	std::thread t = {};
	// ���[�h���
	LoadType loadType;
	// ���[�h���Ă��邩
	bool loadFlag = false;
};