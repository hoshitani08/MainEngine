#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Hunter.h"
#include "FbxObject3d.h"

enum class Phase
{
	Approach,
	Stop,
	Attack,
	Leave
};

enum class AttackType
{
	Weak,
	Ordinary,
	Strong
};

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

public: // �T�u�N���X

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
	const XMFLOAT3& GetPosition() { return monster_->GetPosition(); }
	// HP�̎擾
	int GetHp() { return hp_; }
	// HP�̐ݒ�
	void SetHp(int hp) { hp_ = hp; }
	// ����
	void Move();
	// �v���C���[�̐ݒ�
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// ���񂾂��̎擾
	bool GetIsDead() { return isDead_; }

private: // �����o�ϐ�
	std::unique_ptr<FbxObject3d> monster_;
	Hunter* hunter_ = nullptr;

	float speed_ = 0.5f;
	bool avoidFlag_ = false;
	// �s�������鎞��
	int moveTimer_ = 0;
	// �s�������鎞�Ԃ̍ő�
	int max = 0;
	// �s�������߂�J�E���g
	int count = 0;
	// �s���̎��
	Phase phase_ = Phase::Approach;
	// �U���̎��
	AttackType attackType_ = AttackType::Ordinary;
	//�q�b�g�|�C���g
	int hp_ = 3;
	// ���񂾂�
	bool isDead_ = false;
};