#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Collision.h"
#include "Hunter.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "ObjParticle.h"

class Monster
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

private: // �萔
	// �ŏ��A���O��
	const float MIN_ANGLE = 2;
	// �ő�A���O��
	const float MAX_ANGLE = 10;
	// �ʓx�@
	const float PI = 3.14159265359f;
	// �x���@
	const float ANGLE = 180.0f;

public: // �萔
	const float MAX_HP = 500.0f;

public: // �T�u�N���X
	enum class Phase
	{
		Approach,
		Stop,
		Attack,
		Leave,
		CoolTime
	};

	enum class AttackType
	{
		Weak,
		Ordinary,
		Strong
	};

	enum class AnimationType
	{
		Stop,
		Move,
		Assault,
	};

	enum class PartsDamage
	{
		Body = 10,
		RightForeFoot = 5,
		LeftForeFoot = 5,
		RightHindFoot = 5,
		LeftHindFoot = 5,
		Tail = 8,
	};

public: // �ÓI�����o�֐�
	static std::unique_ptr<Monster> Create(Camera* camera);

public: // �����o�֐�
	// ������
	void Initialize(Camera* camera);
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	// �s��
	void Activity();
	// �����̐ݒ�
	void AngleAdjustment();
	// �U��������������
	void AttackHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f, float damage = 0.0f);
	// �͈͂ɓ�������
	void RangeHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// ����������
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// �_���[�W���󂯂���
	void DamageHit(Sphere hitSphere);
	// �ړ�
	void Move(float speed);
	// ��{�̃A�j���[�V����
	void Animation(AnimationType type);
	// �s���̏I��
	void ActEnd();
	// ���̃p�[�e�B�N��
	void ParticleDraw();
	// ���ʔj��
	void PartsTailDestruction();

public: // �����o�֐�
	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	// X,Y,Z�����̎擾
	const XMFLOAT3& GetRotation() { return nucleus_->GetRotation(); }
	// HP�̎擾
	float GetHp() { return hp_; }
	// ���񂾂��̎擾
	bool GetIsDead() { return isDead_; }

	// �v���C���[�̐ݒ�
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// HP�̐ݒ�
	void SetHp(float hp) { hp_ = hp; }

private: // �����o�ϐ�
	//�j
	std::unique_ptr<Object3d> nucleus_;
	// ��
	std::array<std::unique_ptr<Object3d>, 5> body_;
	//�E�O��
	std::array<std::unique_ptr<Object3d>, 3> rightForeFoot_;
	//���O��
	std::array<std::unique_ptr<Object3d>, 3> leftForeFoot_;
	//�E��둫
	std::array<std::unique_ptr<Object3d>, 3> rightHindFoot_;
	//����둫
	std::array<std::unique_ptr<Object3d>, 3> leftHindFoot_;
	//�K��
	std::array<std::unique_ptr<Object3d>, 4> tail_;
	// �v���C���[�̃f�[�^
	Hunter* hunter_ = nullptr;
	// ���̃p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleManager_;
	std::unique_ptr<ParticleEmitter> blood_;

	std::unique_ptr<ObjParticle> testBlood_;

	// �i�s�����̕ۑ�
	XMFLOAT3 saveVector_ = {};

	// �s�������鎞��
	float moveTimer_ = 0.0f;
	// sin�g�̃^�C�}�[
	float waveTimer_ = 0.0f;
	//�N�[���^�C��
	float coolTimer = 0.0f;
	//�q�b�g�|�C���g
	float hp_ = MAX_HP;

	// �s�������鎞�Ԃ̍ő�
	int maxTime_ = 0;
	// �s�������߂�J�E���g
	int actCount_ = 0;
	//
	int tailDestruction_ = 0;

	// �_���[�W���󂯂���
	bool damageHitFlag_ = false;
	// ����������
	bool hitFlag_ = false;
	// �ۑ��o���邩
	bool saveFlag_ = false;
	// ���񂾂�
	bool isDead_ = false;
	// �s���̏I��
	bool actEndFlag_ = false;
	// �K�����؂ꂽ��
	bool tailDestructionFlag_ = false;

	// �s���̎��
	Phase phase_ = Phase::Approach;
	// �U���̎��
	AttackType attackType_ = AttackType::Ordinary;
};