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
	// �A���O���̐ݒ�
	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }
	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return hunter_->GetPosition(); }
	// X,Y,Z�����̎擾
	const XMFLOAT3& GetRotation() { return hunter_->GetRotation(); }
	// �_���[�W�̃t���O���擾
	bool GetDamageFlag() { return damageFlag_; }
	// �_���[�W�̃t���O��ݒ�
	void SetDamageFlag(bool damageFlag) { damageFlag_ = damageFlag; }
	// ����t���O
	bool GetAvoidFlag() { return avoidFlag_; }
	// ���G���Ԃ��擾
	int GetInvincibleTimer() { return invincibleTimer_; }
	// �U����������
	bool IsAttackFlag() { return isAttackFlag_; }
	// �U���t���O�̐ݒ�
	void AttackHit(bool isAttackFlag);
	// �_�b�V���̃t���O��ݒ�
	///void SetIsDash(bool isDash) { isDash_ = isDash; }
	// �_���[�W�̃p�[�Z���g��ݒ�
	void SetDamage(float damage) { damage_ = damage; }
	// HP�̎擾
	float GetHp() { return hp_; }
	// HP�̐ݒ�
	void SetHp(float hp) { hp_ = hp; }
	// HP�̎擾
	float GetStamina() { return stamina_; }
	// HP�̐ݒ�
	void SetStamina(float stamina) { stamina_ = stamina; }
	//
	void DamageHit();

private: // �����o�ϐ�
	//�@���f��
	std::unique_ptr<FbxObject3d> hunter_;
	// �ړ��{��
	float speed_ = 0.0f;
	// ����t���O
	bool avoidFlag_ = false;
	// ����^�C�}�[
	int avoidTimer_ = 0;
	// �J�����̊p�x
	XMFLOAT2 cameraAngle_ = {};
	// �_���[�W�t���O
	bool damageFlag_ = false;
	// ���G����
	int invincibleTimer_ = 300;
	// �X�^�~�i���c���Ă邩
	bool isStamina_ = true;
	// �U��������������
	bool isAttackFlag_ = false;
	// �U���̃N�[���^�C��
	int attackCoolTimer_ = 0;
	// �q�b�g�|�C���g
	float hp_ = MAX_HP;
	// �X�^�~�i
	float stamina_ = MAX_STAMINA;
	// �_���[�W��
	float damage_ = 0.0f;
};