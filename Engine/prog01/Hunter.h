#pragma once
#include <DirectXMath.h>
#include <memory>

#include "FbxObject3d.h"
#include "Object3d.h"

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

public: // �ÓI�����o�֐�
	static std::unique_ptr<Hunter> Create();

public: // �����o�֐�
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();
	// �s��
	void Behavior();
	// �ړ�
	void BaseMove();
	// ���
	void AvoidMove();
	// �U��
	void AttackMove();
	// �X�s�[�h�v�Z
	void SpeedCalculate();
	// �A�C�e���̎d�l
	void ItemUse();
	// �_���[�W�̌v�Z
	void DamageHit();

public: // �����o�֐�
	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return hunter_->GetPosition(); }
	// X,Y,Z�����̎擾
	const XMFLOAT3& GetRotation() { return hunter_->GetRotation(); }
	// ���G���Ԃ��擾
	int GetInvincibleTimer() { return invincibleTimer_; }
	// �A�C�e���^�C�v�̎擾
	int GetItemType() { return itemType_; }
	// HP�̎擾
	float GetHp() { return hp_; }
	// �X�^�~�i�̎擾
	float GetStamina() { return stamina_; }
	// �U����������
	bool IsAttackFlag() { return isAttackFlag_; }
	// ����t���O
	bool GetAvoidFlag() { return avoidFlag_; }
	// �_���[�W�̃t���O���擾
	bool GetDamageFlag() { return damageFlag_; }

	// �A���O���̐ݒ�
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	// �_���[�W�̃p�[�Z���g��ݒ�
	void SetDamage(float damage) { damage_ = damage; }
	// HP�̐ݒ�
	void SetHp(float hp) { hp_ = hp; }
	// �X�^�~�i�̐ݒ�
	void SetStamina(float stamina) { stamina_ = stamina; }
	// �_���[�W�̃t���O��ݒ�
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	// �U���t���O�̐ݒ�
	void AttackHit(bool isAttackFlag);

private: // �����o�ϐ�
	//�@���f��
	std::unique_ptr<FbxObject3d> hunter_;

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
};