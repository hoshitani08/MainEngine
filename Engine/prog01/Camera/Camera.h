#pragma once

#include <DirectXMath.h>

class Camera
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// �R���X�g���N�^
	Camera(int window_width, int window_height);
	// �f�X�g���N�^
	~Camera() = default;
	// ���t���[���X�V
	void Update();
	// �r���[�s����X�V
	void UpdateViewMatrix();
	// �ˉe�s����X�V
	void UpdateProjectionMatrix();
	// �r���[�s��̎擾
	const XMMATRIX& GetViewMatrix() { return matView_; }
	// �ˉe�s��̎擾
	const XMMATRIX& GetProjectionMatrix() { return matProjection_; }
	// �r���[�ˉe�s��̎擾
	const XMMATRIX& GetViewProjectionMatrix() { return matViewProjection_; }
	// �r���{�[�h�s��̎擾
	const XMMATRIX& GetBillboardMatrix() { return matBillboard_; }
	// ���_���W�̎擾
	const XMFLOAT3& GetEye() { return eye_; }
	// ���_���W�̐ݒ�
	void SetEye(XMFLOAT3 eye) { eye_ = eye; viewDirty_ = true; }
	// �����_���W�̎擾
	const XMFLOAT3& GetTarget() { return target_; }
	// �����_���W�̐ݒ�
	void SetTarget(XMFLOAT3 target) { target_ = target; viewDirty_ = true; }
	// �x�N�g���̎擾
	const XMFLOAT3& GetUp() { return up_; }
	// �x�N�g���̐ݒ�
	void SetUp(XMFLOAT3 up) { up_ = up; viewDirty_ = true; }
	// �x�N�g���ɂ��ړ�
	void CameraMoveVector(const XMFLOAT3& move);
	void CameraMoveEyeVector(const XMFLOAT3& move);
	void CameraMoveTargetVector(const XMFLOAT3& move);

protected: // �����o�ϐ�
	// �r���[�s��
	XMMATRIX matView_ = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	XMMATRIX matBillboard_ = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY_ = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection_ = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection_ = DirectX::XMMatrixIdentity();
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty_ = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty_ = false;
	// ���_���W
	XMFLOAT3 eye_ = { 0, 0, -50 };
	// �����_���W
	XMFLOAT3 target_ = { 0, 0, 0 };
	// ������x�N�g��
	XMFLOAT3 up_ = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio_ = 1.0f;
};