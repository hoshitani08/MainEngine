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

private: // �V���O���g��
	SceneManager() = default;
	~SceneManager();

public: // �V���O���g��
	SceneManager(const SceneManager& sceneManager) = delete;
	SceneManager& operator=(const SceneManager& sceneManager) = delete;
	static SceneManager* GetInstance();

public:
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
	// ���[�h�t���O�̐ݒ�
	void SetLockFlag(bool isLoaded);
	// ���[�h�t���O�̎擾
	bool GetLockFlag();
	//  �񓯊����[�h
	void AsyncLoad();

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
	std::thread th_ = {};
	// ���[�h���
	LoadType loadType_;
	// ���[�h���Ă��邩
	bool isLoaded_ = false;
	// �X���b�h�ԂŎg�p���鋤�L���\�[�X��r�����䂷��
	std::mutex isLoadedMutex;
};