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
	enum class AnimationType
	{
		InitialPosture,
		Move,
		Assault,
		TailAttack,
		Punch,
		Waiting,
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
	/// <returns></returns>
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="hitSphere">��</param>
	void DamageHit(Sphere hitSphere);
	/// <summary>
	/// ��{�̃A�j���[�V����
	/// </summary>
	/// <param name="type">�A�j���[�V�����^�C�v</param>
	void Animation(AnimationType type);
	/// <summary>
	/// ���ʔj��
	/// </summary>
	void PartsTailDestruction();
	/// <summary>
	/// �r�w�C�r�A�c���[
	/// </summary>
	void BehaviorTree();
	/// <summary>
	/// ���K
	/// </summary>
	/// <returns></returns>
	bool Howl();
	/// <summary>
	/// �U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode();
	/// <summary>
	/// �ҋ@
	/// </summary>
	/// <returns></returns>
	bool WaitingMode();
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
	/// �p���`�U��
	/// </summary>
	/// <returns></returns>
	bool AttackMode3();
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
	std::unique_ptr<ParticleEmitter> bloodEmitter_;
	std::unique_ptr<ObjParticle> bloodParticle_;
	// �A�̃p�[�e�B�N��
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
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
#pragma endregion �X�e�[�^�X
#pragma region 
	// sin�g�̃^�C�}�[
	float waveTimer_ = 0.0f;
	// �C�[�W���O�p�ʒu
	XMFLOAT3 endRotation_ = {0,360,0};
	// �C�[�W���O�̐i�s�x�p
	float easeTimer_ = 0.0f;
	float angleEaseTimer_ = 0.0f;
	// �C�[�W���O���I�������
	bool isEaseFlag_ = false;
	// �����A���O��
	float addAngle = 5.0f;
	// �p���`���I�������
	bool isPunch = false;
	//
	int count = 0;
	int maxCount = 0;
#pragma endregion �A�j���[�V����
#pragma region 
	// �s���̃Z���N�^�[
	std::vector<std::function<bool()>> activitySelector_;
	// �U���̃V�[�N�G���X
	std::vector<std::function<bool()>> attackSequence_;
	// �ҋ@�̃V�[�N�G���X
	std::vector<std::function<bool()>> waitingSequence_;
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
#pragma region 
	//
	bool debugFlag = false;
#pragma endregion �f�o�b�N
};