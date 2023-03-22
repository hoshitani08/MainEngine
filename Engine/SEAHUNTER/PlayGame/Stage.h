#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "Object3d.h"
#include "Block.h"
#include "Hunter.h"
#include "Monster.h"
#include "ObjParticle.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "LevelLoader.h"

class Stage
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage(Monster* monster, Hunter* hunter, Camera* camera, const std::string& fName);
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const std::string& fName);
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
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �����̃p�[�e�B�N��
	/// </summary>
	void SandParticle();

private: // �����o�ϐ�
	// json�f�[�^
	std::unique_ptr<LevelData> levelData;
	//
	std::vector<std::unique_ptr<Object3d>> objects;
	// ���X��̃I�u�W�F�N�g
	std::vector<std::unique_ptr<Block>> block_;
	// �����X�^�[�̃|�C���^
	Monster* monster_ = nullptr;
	// �n���^�[�̃|�C���^
	Hunter* hunter_ = nullptr;
	//�J����
	Camera* camera_ = nullptr;
	// �����̃G�~�b�^�[
	std::unique_ptr<ParticleEmitter> fugitiveBustEmitter;
	// �����̃p�[�e�B�N��
	std::unique_ptr<ParticleManager> fugitiveBustParticle;
};

