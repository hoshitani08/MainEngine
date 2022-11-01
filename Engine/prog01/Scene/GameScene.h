#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CollisionPrimitive.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "Hunter.h"
#include "Monster.h"
#include "UserInterface.h"
#include "Block.h"

class CollisionManager;
class Player;
class ContactableObject;

class GameScene : public BaseScene
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

public: // �T�u�N���X
	struct Quest
	{
		int timer = 0;
		int second = 0;
		int minute = 0;
	};

private: // �ÓI�萔
	static const int DEBUG_TEXT_TEX_NUMBER = 0;

private: // �萔
	const XMFLOAT2 RESTRICTION_ANGLE = { 360.0f, 80.0f };

public: // �����o�֐�
	// �f�X�g���N�^
	~GameScene();
	// ������
	void Initialize() override;
	//�I��
	void Finalize() override;
	// ���t���[������
	void Update() override;
	// �`��
	void Draw() override;
	// �G�t�F�N�g�`��
	void EffectDraw() override;
	// �J�����̓���
	void CameraMove();
	// �v���C���[�̍U������
	void PlayerAttack();

private: // �����o�ϐ�
	//�J����
	std::unique_ptr<Camera> camera_;
	//�X�v���C�g
	std::unique_ptr<Sprite> sprite_;
	//UI
	std::unique_ptr<UserInterface> ui_;
	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleMan_;
	//�I�u�W�F�N�g
	std::unique_ptr<Object3d> skydome_;
	std::unique_ptr<Object3d> ground_;
	std::unique_ptr<Hunter> hunter_;
	std::unique_ptr<Monster> monster_;
	std::unique_ptr<Object3d> hitSphere_;

	std::unique_ptr<Block> block_;

	//���C�g
	std::unique_ptr<LightGroup> light_;
	//�������������l
	float circleShadowDir_[3] = { 0,-1,0 };
	float circleShadowPos_[3] = { 1,2,0 };
	float circleShadowAtten_[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle_[2] = { 0.0f, 0.5f };

	float fighterPos[3] = { 1, 0.0f, 0 };
	//�����蔻��
	CollisionManager* collisionManager_ = nullptr;
	// �p�x
	XMFLOAT2 angle_ = { 0.0f, 0.0f};

	// �^�C�}�[
	Quest quest_;
};