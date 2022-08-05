#pragma once
#include <DirectXMath.h>
#include <memory>

#include "FbxObject3d.h"

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

	void Move();
	void Attack();

	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position_; }
	// �_���[�W�̃p�[�Z���g���擾
	float GetDamagePercent() { return damagePercent_; }
	// �_���[�W�̃t���O���擾
	bool GetDamageFlag() { return damageFlag_; }
	// ����t���O
	bool GetAvoidFlag() { return avoidFlag_; }
	// ���G���Ԃ��擾
	int GetInvincibleTimer() { return invincibleTimer_; }
	//�X�^�~�i�̌����l���擾
	float GetStrengthDecrement() { return strengthDecrement; }
	// �_���[�W�̃t���O��ݒ�
	void SetDamageFlag(bool damageFlag);
	// �_���[�W�̃p�[�Z���g��ݒ�
	void SetDamagePercent(float damagePercent) { damagePercent_ = damagePercent; }

private: // �����o�ϐ�
	//�@���f��
	std::unique_ptr<FbxObject3d> hunter_;
	//�@�ʒu
	XMFLOAT3 position_;
	//�@�ړ��{��
	float speed_ = 0.0f;
	//�@����t���O
	bool avoidFlag_ = false;
	//�@����^�C�}�[
	int avoidTimer_ = 0;
	//�@�J�����̊p�x
	XMFLOAT2 cameraAngle_ = {};
	// �_���[�W
	float damagePercent_ = 0.0f;
	// �_���[�W�t���O
	bool damageFlag_ = false;
	// ���G����
	int invincibleTimer_ = 300;
	// �X�^�~�i�̌����l
	float strengthDecrement = 0.0f;
};