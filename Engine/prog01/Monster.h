#pragma once
#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Hunter.h"
#include "FbxObject3d.h"



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

public: // �ÓI�����o�֐�
	static std::unique_ptr<Monster> Create();

public: // �����o�֐�
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return nucleus_->GetPosition(); }
	// HP�̎擾
	int GetHp() { return hp_; }
	// HP�̐ݒ�
	void SetHp(int hp) { hp_ = hp; }
	// �s��
	void Activity();
	// �v���C���[�̐ݒ�
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// ���񂾂��̎擾
	bool GetIsDead() { return isDead_; }
	// �����̐ݒ�
	void AngleAdjustment();
	// �U��������������
	void DamageHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f, float damage = 0.0f);
	// �͈͂ɓ�������
	void RangeHit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);
	//
	bool Hit(XMFLOAT3 partsPosition, float enemyRange = 1.0f, float playerRange = 1.0f);

	// �ړ�
	void Move(float speed);
	// ��{�̃A�j���[�V����
	void Animation(AnimationType type);
	// �s���̏I��
	void ActEnd();

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

	// �i�s�����̕ۑ�
	XMFLOAT3 saveVector_ = {};
	// �v���C���[�̈ʒu�̕ۑ�
	XMFLOAT3 savePlayerPosition = {};

	// ���Z�A���O��
	float addAngle_ = 2.0f;
	// �s�������鎞��
	float moveTimer_ = 0.0f;
	// sin�g�̃^�C�}�[
	float waveTimer_ = 0.0f;
	//�N�[���^�C��
	float coolTimer = 0.0f;

	// �s�������鎞�Ԃ̍ő�
	int maxTime_ = 0;
	// �s�������߂�J�E���g
	int actCount_ = 0;
	//�q�b�g�|�C���g
	int hp_ = 3;

	// ����������
	bool hitFlag_ = false;
	// �ۑ��o���邩
	bool saveFlag_ = false;
	// ���񂾂�
	bool isDead_ = false;
	// �s���̏I��
	bool actEndFlag_ = false;

	// �s���̎��
	Phase phase_ = Phase::Approach;
	// �U���̎��
	AttackType attackType_ = AttackType::Ordinary;
};