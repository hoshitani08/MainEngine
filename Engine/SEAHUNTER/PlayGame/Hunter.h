#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "FbxObject3d.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "ObjParticle.h"
#include "Ease.h"
#include "Collision.h"

class Hunter
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

public: // �萔
	const float MAX_HP = 150.0f;
	const float MAX_STAMINA = 150.0f;
	const float ANGLE_RESTRICTION = 0.6f;

public: // �T�u�N���X
	// �A�j���[�V�����̃^�C�v
	enum class AnimationType
	{
		HALT,
		MOVE,
		DAMAGE,
		DEATH,
		DASH,
		DODGE,
		COMBO1,
		COMBO2,
		COMBO3,
	};

	// �A�j���[�V�����t���O
	struct AnimationFlag
	{
		bool halt = false;
		bool move = false;
		bool damage = false;
		bool attack1 = false;
		bool attack2 = false;
		bool attack3 = false;
		bool death = false;
		bool dash = false;
		bool dodge = false;
	};

	// �^�C���f�[�^
	struct TimeData
	{
		int count = 0;
		int timer = 0;
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	/// <returns>Hunter</returns>
	static std::unique_ptr<Hunter> Create();

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
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
	/// �s��
	/// </summary>
	void Behavior();
	/// <summary>
	/// �ړ�
	/// </summary>
	void BaseMove();
	/// <summary>
	/// ���
	/// </summary>
	void AvoidMove();
	/// <summary>
	/// �U��
	/// </summary>
	void AttackMove();
	/// <summary>
	/// �X�s�[�h�v�Z
	/// </summary>
	void SpeedCalculate();
	/// <summary>
	/// �A�C�e���̎d�l
	/// </summary>
	void ItemUse();
	/// <summary>
	/// �_���[�W�̌v�Z
	/// </summary>
	void DamageHit();

public: // �����o�֐�
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return hunter_[animationType_]->GetPosition(); }
	/// <summary>
	/// X,Y,Z�����̎擾
	/// </summary>
	/// <returns>X,Y,Z�����̉�]</returns>
	const XMFLOAT3& GetRotation() { return hunter_[animationType_]->GetRotation(); }
	/// <summary>
	/// ���G���Ԃ��擾
	/// </summary>
	/// <returns>���G����</returns>
	int GetInvincibleTimer() { return invincibleTimer_; }
	/// <summary>
	/// �A�C�e���^�C�v�̎擾
	/// </summary>
	/// <returns>�A�C�e���^�C�v</returns>
	int GetItemType() { return itemType_; }
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return hp_; }
	/// <summary>
	/// �X�^�~�i�̎擾
	/// </summary>
	/// <returns>�X�^�~�i</returns>
	float GetStamina() { return stamina_; }
	/// <summary>
	/// �U���t���O�̎擾
	/// </summary>
	/// <returns>�U���t���O</returns>
	bool IsAttackFlag() { return isAttackFlag_; }
	/// <summary>
	/// ����t���O�̎擾
	/// </summary>
	/// <returns>����t���O</returns>
	bool GetAvoidFlag() { return avoidFlag_; }
	/// <summary>
	/// �_���[�W�t���O���擾
	/// </summary>
	/// <returns>�_���[�W�t���O</returns>
	bool GetDamageFlag() { return damageFlag_; }
	/// <summary>
	/// �U���A�j���[�V�����������Ă��邩
	/// </summary>
	/// <returns>�U���t���O</returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// �A�C�e����I�����Ă��邩
	/// </summary>
	/// <returns>�I���t���O</returns>
	bool GetItemSelectionFlag() { return itemSelectionFlag_; }
	/// <summary>
	/// �����t���O�̎擾
	/// </summary>
	/// <returns>�����t���O</returns>
	bool GetIsDeath() { return isDeath_; }
	/// <summary>
	/// �A�C�e���I���̃C�[�W���O���I�������
	/// </summary>
	/// <returns></returns>
	bool GetIsItemSelection() { return isItemSelectionEase_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetButtonFlag() { return buttonFlag_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetButtonEaseFlag() { return buttonEaseFlag_; }
	/// <summary>
	/// �����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsMoveFlag()
	{
		if (falg_.move || falg_.dash)
		{
			return true;
		}
		return false;
	}
	/// <summary>
	/// �A���O���̐ݒ�
	/// </summary>
	/// <param name="angle">�A���O��</param>
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	/// <summary>
	/// �_���[�W�̃p�[�Z���g��ݒ�
	/// </summary>
	/// <param name="damage">�_���[�W</param>
	void SetDamage(float damage) { damage_ = damage; }
	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHp(float hp) { hp_ = hp; }
	/// <summary>
	/// �X�^�~�i�̐ݒ�
	/// </summary>
	/// <param name="stamina">�X�^�~�i</param>
	void SetStamina(float stamina) { stamina_ = stamina; }
	/// <summary>
	/// �_���[�W�t���O��ݒ�
	/// </summary>
	/// <param name="damageFlag">�_���[�W�t���O</param>
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	/// <summary>
	/// �U���t���O�̐ݒ�
	/// </summary>
	/// <param name="isAttackFlag">�U���t���O</param>
	void SetAttackFlag(bool isAttackFlag);
	/// <summary>
	/// �R���{�p
	/// </summary>
	/// <param name="f"></param>
	void SetActFalg(bool f) { actFlag_ = f; }
	/// <summary>
	/// �A�C�e���I���̃C�[�W���O���I�������
	/// </summary>
	/// <param name="f"></param>
	void SetIsItemSelection(bool f) { isItemSelectionEase_ = f; }
	/// <summary>
	/// �{�^���t���O�̐ݒ�
	/// </summary>
	/// <param name="f"></param>
	void SetButtonFlag(bool f) { buttonFlag_ = f; }
	/// <summary>
	/// �{�^���p�̃C�[�W���O�f�[�^�t���O�̐ݒ�
	/// </summary>
	/// <param name="f"></param>
	void SetButtonEaseFlag(bool f) { buttonEaseFlag_ = f; }
	/// <summary>
	/// �U���������̓����蔻��̎擾
	/// </summary>
	/// <returns>����</returns>
	Sphere GetAttackHit();

private: // �����o�֐�(�A�j���[�V�����p)
	/// <summary>
	/// ��~
	/// </summary>
	void Halt();
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	void Damage();
	/// <summary>
	/// ���S
	/// </summary>
	void Death();
	/// <summary>
	/// �_�b�V��
	/// </summary>
	void Dash();
	/// <summary>
	/// ���
	/// </summary>
	void Dodge();
	/// <summary>
	/// �R���{�U��1
	/// </summary>
	void Combo1();
	/// <summary>
	/// �R���{�U��2
	/// </summary>
	void Combo2();
	/// <summary>
	/// �R���{�U��3
	/// </summary>
	void Combo3();

private: // �����o�ϐ�
	//�@���f��
	std::array<std::unique_ptr<FbxObject3d>, 9> hunter_;
	std::unique_ptr<Object3d> weapon_;
	// �A�C�e���p�p�[�e�B�N��
	std::unique_ptr<ParticleEmitter> itemEmitter_;
	std::unique_ptr<ObjParticle> itemParticle_;
	std::unique_ptr<ParticleEmitter> healEmitter_;
	std::unique_ptr<ObjParticle> healParticle_;

	// �A�j���[�V�����֐��̊Ǘ�
	std::vector<std::function<void()>> animationFunc_;

	// �J�����̊p�x
	XMFLOAT2 cameraAngle_ = {};
	XMFLOAT2 playerAngle_ = {};

	// ����^�C�}�[
	int avoidTimer_ = 0;
	// ���G����
	int invincibleTimer_ = 300;
	// �U���̃N�[���^�C��
	int attackCoolTimer_ = 0;
	// �A�C�e���̃^�C�v
	int itemType_ = 0;
	// �A�j���[�V�����̃^�C�v
	int animationType_ = 0;
	// �U���̌��ʂ̎���
	TimeData attack;
	// �U���̌��ʂ̎���
	TimeData defense;
	// �A�j���[�V�����p�̃C�[�W���O�f�[�^
	std::unique_ptr<EaseData> animationEaseData_;

	// �ړ��{��
	XMFLOAT2 speed_ = {};
	// �q�b�g�|�C���g
	float hp_ = MAX_HP;
	// �X�^�~�i
	float stamina_ = MAX_STAMINA;
	// �_���[�W��
	float damage_ = 0.0f;

	// ����t���O
	bool avoidFlag_ = false;
	// �_���[�W�t���O
	bool damageFlag_ = false;
	// �X�^�~�i���c���Ă邩
	bool isStamina_ = true;
	// �U��������������
	bool isAttackFlag_ = false;
	// �A�C�e���̑I�����Ă邩
	bool itemSelectionFlag_ = false;
	// �A�C�e���I���̃C�[�W���O���I�������
	bool isItemSelectionEase_ = false;
	// �����Ă��邩�ǂ���
	bool isDeath_ = false;
	// �_�b�V�����Ă��邩
	bool isDash_ = false;
	// �U�����p�����邩
	bool comboFlag_ = false;
	// �U�����؂ꂽ��
	bool actFlag_ = false;
	// �{�^��
	bool buttonFlag_ = false;
	// �{�^���̃C�[�W���O
	bool buttonEaseFlag_ = false;
	// �����Ă��邩
	bool isMoveFlag_ = false;
	// �A�j���[�V�����̃^�C�v�t���O
	AnimationFlag falg_ = {};
};