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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">�\�����WX</param>
	/// <param name="window_height">�\�����WY</param>
	Camera(int window_width, int window_height);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Camera() = default;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateViewMatrix();
	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateProjectionMatrix();
	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	const XMMATRIX& GetViewMatrix() { return matView_; }
	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	const XMMATRIX& GetProjectionMatrix() { return matProjection_; }
	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	const XMMATRIX& GetViewProjectionMatrix() { return matViewProjection_; }
	/// <summary>
	/// �r���{�[�h�s��̎擾
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	const XMMATRIX& GetBillboardMatrix() { return matBillboard_; }
	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���_���W</returns>
	const XMFLOAT3& GetEye() { return eye_; }
	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="eye">���_���W</param>
	void SetEye(XMFLOAT3 eye) { eye_ = eye; viewDirty_ = true; }
	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>�����_���W</returns>
	const XMFLOAT3& GetTarget() { return target_; }
	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="target">�����_���W</param>
	void SetTarget(XMFLOAT3 target) { target_ = target; viewDirty_ = true; }
	/// <summary>
	/// �x�N�g���̎擾
	/// </summary>
	/// <returns>�x�N�g��</returns>
	const XMFLOAT3& GetUp() { return up_; }
	/// <summary>
	/// �x�N�g���̐ݒ�
	/// </summary>
	/// <param name="up">�x�N�g��</param>
	void SetUp(XMFLOAT3 up) { up_ = up; viewDirty_ = true; }
	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�x�N�g��</param>
	void CameraMoveVector(const XMFLOAT3& move);
	/// <summary>
	/// �x�N�g���ɂ��ړ�(Eye�̂�)
	/// </summary>
	/// <param name="move">�x�N�g��</param>
	void CameraMoveEyeVector(const XMFLOAT3& move);
	/// <summary>
	/// �x�N�g���ɂ��ړ�(Target�̂�)
	/// </summary>
	/// <param name="move">�x�N�g��</param>
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