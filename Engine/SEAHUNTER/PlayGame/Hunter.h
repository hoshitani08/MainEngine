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

public: // �T�u�N���X
	struct AnimationFlag
	{
		bool halt = false;
		bool move = false;
		bool damage = false;
		bool attack = false;
		bool death = false;
		bool dash = false;
		bool dodge = false;
	};

	struct TimeData
	{
		int count = 0;
		int timer = 0;
	};

public: // �ÓI�����o�֐�
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
	const XMFLOAT3& GetPosition() { return hunter_[0]->GetPosition(); }
	/// <summary>
	/// X,Y,Z�����̎擾
	/// </summary>
	/// <returns>X,Y,Z�����̉�]</returns>
	const XMFLOAT3& GetRotation() { return hunter_[0]->GetRotation(); }
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
	bool GetAnimationType() { return falg_.attack; }
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
	void AttackHit(bool isAttackFlag);

private: // �����o�ϐ�
	//�@���f��
	std::array<std::unique_ptr<FbxObject3d>, 7> hunter_;
	std::unique_ptr<Object3d> buki_;
	// �A�C�e���p�p�[�e�B�N��
	std::unique_ptr<ParticleEmitter> itemEmitter_;
	std::unique_ptr<ObjParticle> itemParticle_;
	std::unique_ptr<ParticleEmitter> healEmitter_;
	std::unique_ptr<ObjParticle> healParticle_;

	// �J�����̊p�x
	XMFLOAT2 cameraAngle_ = {};

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

	// �ړ��{��
	float speed_ = 0.0f;
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
	// �����Ă��邩�ǂ���
	bool isDeath_ = false;
	//
	bool isDash = false;
	// �A�j���[�V�����̃^�C�v�t���O
	AnimationFlag falg_ = {};
};