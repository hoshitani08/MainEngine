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
#include "Vector.h"
#include "BehaviorTree.h"

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
	// �ʓx�@
	const float PI = 3.14159265359f;
	// �x���@
	const float ANGLE = 180.0f;

public: // �萔
	const float MAX_HP = 500.0f;

public: // �T�u�N���X
	// �A�j���[�V�����̃^�C�v
	enum class AnimationType
	{
		InitialPosture,
		Move,
		Assault,
		TailAttack,
		RightPunch,
		LeftPunch,
		Waiting,
		Tornado,
	};

	// �p�[�c���ƂɎ󂯂�_���[�W�̐ݒ�
	enum class PartsDamage
	{
		Body = 10,
		RightForeFoot = 5,
		LeftForeFoot = 5,
		RightHindFoot = 5,
		LeftHindFoot = 5,
		Tail = 8,
	};

	// �p�[�c
	enum class Parts
	{
		Body,
		RightForeFoot,
		LeftForeFoot,
		RightHindFoot,
		LeftHindFoot,
		Tail,
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="hunter">�v���C���[</param>
	/// <returns>�������ꂽ�I�u�W�F�N�g</returns>
	static std::unique_ptr<Monster> Create(Camera* camera, Hunter* hunter);

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="camera">�J����</param>
	void Initialize(Camera* camera);
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
	/// �S�Ă̓���
	/// </summary>
	void AllMove();
	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	void AngleAdjustment();
	/// <summary>
	/// �����蔻��
	/// </summary>
	/// <param name="partsPosition">���W</param>
	/// <param name="enemyRange">���a</param>
	/// <param name="playerRange">���a</param>
	/// <returns>�����������ǂ���</returns>
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="hitSphere">��</param>
	void DamageHit(Sphere hitSphere);
	/// <summary>
	/// ���ʔj��
	/// </summary>
	void PartsTailDestruction();
	/// <summary>
	/// ���K
	/// </summary>
	/// <returns></returns>
	bool Howl();
	/// <summary>
	/// ���S
	/// </summary>
	/// <returns></returns>
	bool Dead();
	/// <summary>
	/// �U���̌o�ߎ���
	/// </summary>
	/// <returns></returns>
	bool AttackElapsedTime();
	/// <summary>
	/// �U���̑I��
	/// </summary>
	/// <returns></returns>
	bool AttackModeSelection();
	/// <summary>
	/// �U���̎��s
	/// </summary>
	/// <returns></returns>
	bool AttackModeMove();
	/// <summary>
	/// �ːi
	/// </summary>
	/// <returns></returns>
	bool AttackMode1();
	/// <summary>
	/// �K���U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode2();
	/// <summary>
	/// �E�p���`�U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode3();
	/// <summary>
	/// ���p���`�U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode4();
	/// <summary>
	/// �͈͍U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode5();
	/// <summary>
	/// �ҋ@�̌o�ߎ���
	/// </summary>
	/// <returns></returns>
	bool WaitingElapsedTime();
	/// <summary>
	/// �ҋ@�̑I��
	/// </summary>
	/// <returns></returns>
	bool WaitingModeSelection();
	/// <summary>
	/// �ҋ@�̎��s
	/// </summary>
	/// <returns></returns>
	bool WaitingModeMove();
	/// <summary>
	/// �������Ȃ�
	/// </summary>
	/// <returns></returns>
	bool WaitingMode1();
	/// <summary>
	/// �ڋ�
	/// </summary>
	/// <returns></returns>
	bool WaitingMode2();
	/// <summary>
	/// �ԍ��������
	/// </summary>
	/// <returns></returns>
	bool WaitingMode3();
	/// <summary>
	/// �r�w�C�r�A�c���[�̃��Z�b�g
	/// </summary>
	void TreeReset();
	/// <summary>
	/// �_���[�W�v�Z
	/// </summary>
	/// <param name="partsDamage">�_���[�W��</param>
	/// <param name="parts">�ǂ��̕��ʂ�</param>
	/// <param name="hitSphere">����</param>
	/// <param name="temp">���ʂ̃I�u�W�F�N�g</param>
	void DamageCalculate(float partsDamage, Parts parts, Sphere hitSphere, Object3d* temp);
	/// <summary>
	/// �؂ꂽ�K���ɂ��U��
	/// </summary>
	void TailBullet();

public: // �����o�֐�
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	/// <summary>
	/// X,Y,Z�����̎擾
	/// </summary>
	/// <returns>X,Y,Z�����̉�]</returns>
	const XMFLOAT3& GetRotation() { return nucleus_->GetRotation(); }
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return hp_; }
	/// <summary>
	/// ���S�t���O�̎擾
	/// </summary>
	/// <returns>���S�t���O</returns>
	bool GetIsDead() { return isDead_; }
	/// <summary>
	/// �v���C���[�̐ݒ�
	/// </summary>
	/// <param name="hunter">�v���C���[</param>
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHp(float hp) { hp_ = hp; }

private: // �����o�֐�(�A�j���[�V�����p)
	/// <summary>
	/// �����p��
	/// </summary>
	void InitialPosture();
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// �ːi
	/// </summary>
	void Assault();
	/// <summary>
	/// �K���U��
	/// </summary>
	void TailAttack();
	/// <summary>
	/// �E�p���`
	/// </summary>
	void RightPunch();
	/// <summary>
	/// ���p���`
	/// </summary>
	void LeftPunch();
	/// <summary>
	/// �ҋ@
	/// </summary>
	void Waiting();
	/// <summary>
	/// �g���l�[�h
	/// </summary>
	void Tornado();

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
	// ���̃G�~�b�^�[
	std::unique_ptr<ParticleEmitter> bloodEmitter_;
	// ���̃p�[�e�B�N��
	std::unique_ptr<ObjParticle> bloodParticle_;
	// �A�̃G�~�b�^�[
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
	// �A�̃p�[�e�B�N��
	std::unique_ptr<ObjParticle> bubbleParticle_;
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
	// �J���[��ς��邽�߂̃t���O
	bool colorChangeFlag_ = false;
	// �K���������܂ł̃^�C�}�[
	int tailMoveTimer_ = 0;
	// �͈͍U���͈̔�
	float attackRange_ = 1.0f;
	//
	int tornadoTimer_ = 0;
	// �K���̈ړ��̕����̃��Z�b�g�t���O
	std::array<bool, 4> tailMoveResetFlag_;
	std::array<bool, 4> tailMoveChangeFlag_;
	// �K���������ړ��ʂ̕ۑ�
	XMFLOAT3 saveTailMoveVector_ = {};
#pragma endregion �X�e�[�^�X
#pragma region 
	// �A�j���[�V�����֐��̊Ǘ�
	std::vector<std::function<void()>> animationFunc_;
	// sin�g�̃^�C�}�[
	float waveTimer_ = 0.0f;
	// �C�[�W���O�̐i�s�x�p
	float easeTimer_ = 0.0f;
	float angleEaseTimer_ = 0.0f;
	// �C�[�W���O���I�������
	bool isEaseFlag_ = false;
	// �����A���O��
	float addAngle_ = 5.0f;
	// �p���`���I�������
	bool isPunch_ = false;
	// �s���̌o�߂����J�E���g
	int count_ = 0;
	// �s���̌o�߂����J�E���g�ő�
	int maxCount_ = 0;
	// �F�ω��̃^�C�}�[
	int colorTimer_ = 0;
	// ���K���鎞��
	int howlTimer_ = 0;
#pragma endregion �A�j���[�V����
#pragma region 
	// �r�w�C�r�A�c���[
	std::unique_ptr<BehaviorTree> behaviorTree_;
	// �U���̃Z���N�^�[
	std::vector<std::function<bool()>> attackSelector_;
	std::vector<bool> attackSelect_;
	// �ҋ@�̃Z���N�^�[
	std::vector<std::function<bool()>> waitingSelector_;
	std::vector<bool> waitingSelect_;
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