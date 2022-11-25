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
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static SceneManager* GetInstance();

public:
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �G�t�F�N�g����`��
	/// </summary>
	void EffectDraw();
	/// <summary>
	/// ���V�[���̗\��
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	void ChangeScene(const std::string& sceneName);
	/// <summary>
	/// �V�[�������̐ݒ�
	/// </summary>
	/// <param name="sceneFactory">�V�[������</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:
	/// <summary>
	/// ���[�h�V�[���̐ݒ�
	/// </summary>
	void CreateLoadScene();
	/// <summary>
	/// ���[�h�t���O�̐ݒ�
	/// </summary>
	/// <param name="isLoaded"></param>
	void SetLockFlag(bool isLoaded);
	/// <summary>
	/// ���[�h�t���O�̎擾
	/// </summary>
	/// <returns></returns>
	bool GetLockFlag();
	/// <summary>
	/// �񓯊����[�h
	/// </summary>
	void AsyncLoad();

private:
	//���̃V�[��
	std::unique_ptr<BaseScene> scene_;
	//���̃V�[��
	std::unique_ptr<BaseScene> nextScene_;
	//���[�h�V�[��
	std::unique_ptr<BaseScene> loadScene_;
	//�V�[���t�@�N�g��
	AbstractSceneFactory* sceneFactory_ = nullptr;
	// �񓯊�����
	std::thread th_ = {};
	// ���[�h���
	LoadType loadType_ = LoadType::NoLoad;
	// ���[�h���Ă��邩
	bool isLoaded_ = false;
	// �X���b�h�ԂŎg�p���鋤�L���\�[�X��r�����䂷��
	std::mutex isLoadedMutex = {};
};