#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Collision.h"
#include "Hunter.h"
#include "Object3d.h"
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
	const float MIN_ANGLE = 2.0f;
	// �ő�A���O��
	const float MAX_ANGLE = 10.0f;
	// �ʓx�@
	const float PI = 3.14159265359f;
	// �x���@
	const float ANGLE = 180.0f;

public: // �萔
	const float MAX_HP = 500.0f;

public: // �T�u�N���X
	enum class AnimationType
	{
		Stop,
		Move,
		Assault,
		TailAttack,
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
	static std::unique_ptr<Monster> Create(Camera* camera, Hunter* hunter);

public: // �����o�֐�
	// ������
	void Initialize(Camera* camera);
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// �����̐ݒ�
	void AngleAdjustment();
	// ����������
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	// �_���[�W���󂯂���
	void DamageHit(Sphere hitSphere);
	// ��{�̃A�j���[�V����
	void Animation(AnimationType type);
	// ���ʔj��
	void PartsTailDestruction();
	// �r�w�C�r�A�c���[
	void BehaviorTree();
	// ���K
	bool Howl();
	// �U��
	bool AttackMode();
	// �ҋ@
	bool WaitingMode();
	// ���S
	bool Dead();
	// �U���̌o�ߎ���
	bool AttackElapsedTime();
	// �U���̑I��
	bool AttackModeSelection();
	// �U���̎��s
	bool AttackModeMove();
	// �ːi
	bool AttackMode1();
	// �K���U��
	bool AttackMode2();
	// �ҋ@�̌o�ߎ���
	bool WaitingElapsedTime();
	// �ҋ@�̑I��
	bool WaitingModeSelection();
	// �ҋ@�̎��s
	bool WaitingModeMove();
	// �������Ȃ�
	bool WaitingMode1();
	// �ڋ�
	bool WaitingMode2();
	// �ԍ��������
	bool WaitingMode3();
	// �r�w�C�r�A�c���[�̃��Z�b�g
	void TreeReset();

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
#pragma region
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
	std::unique_ptr<ParticleEmitter> blood_;
	std::unique_ptr<ObjParticle> testBlood_;
#pragma endregion ���f��
#pragma region
	// �K���̑ϋv�l
	int tailDestruction_ = 0;
	//�q�b�g�|�C���g
	float hp_ = MAX_HP;
	// �i�s�����̕ۑ�
	XMFLOAT3 saveVector_ = {};
	// �_���[�W���󂯂���
	bool damageHitFlag_ = false;
	// ����������
	bool hitFlag_ = false;
	// ���񂾂�
	bool isDead_ = false;
	// �K�����؂ꂽ��
	bool tailDestructionFlag_ = false;
#pragma endregion �X�e�[�^�X
#pragma region 
	// sin�g�̃^�C�}�[
	float waveTimer_ = 0.0f;
#pragma endregion �A�j���[�V����
#pragma region 
	// �s���̃Z���N�^�[
	std::vector<std::function<bool()>> activitySelector_;
	// �U���̃V�[�N�G���X
	std::vector<std::function<bool()>> attackSequence_;
	// �ҋ@�̃V�[�N�G���X
	std::vector<std::function<bool()>> waitingSequence_;
	// �U���̃Z���N�^�[
	std::vector<bool> attackSelector_;
	// �ҋ@�̃Z���N�^�[
	std::vector<bool> waitingSelector_;
	// �U���̌o�ߎ���
	int attackElapsedTimer_ = 0;
	// �ҋ@�̌o�ߎ���
	int waitingElapsedTimer_ = 0;
	// ���K������
	bool howlflag_ = false;
	// �ǔ��̏I��
	bool trackingEnd_ = false;
	// �U���̏I��
	bool attackEnd_ = false;
	// �ҋ@�̏I��
	bool waitingEnd_ = false;
#pragma endregion �r�w�C�r�A�c���[
};