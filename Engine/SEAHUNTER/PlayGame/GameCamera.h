#pragma once

#include "Camera.h"
#include "Hunter.h"
#include "Ease.h"

class GameCamera
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
	const XMFLOAT2 RESTRICTION_ANGLE = { 360.0f, 80.0f };

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameCamera();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameCamera();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �J�����̃A���O��
	/// </summary>
	void CameraAngle(XMFLOAT2 angle);
	/// <summary>
	/// �J�n���̃J�������o
	/// </summary>
	void StratCameraMove();
	/// <summary>
	/// �Q�[�����̃J��������
	/// </summary>
	void GamePlayCameraMove();
	/// <summary>
	/// �I�����̃J�������o
	/// </summary>
	void EndCameraMove();
	/// <summary>
	/// �J�������Z�b�g
	/// </summary>
	void CameraReset();
	/// <summary>
	/// �J�����̃C�[�W���O���I�������
	/// </summary>
	/// <returns>�t���O</returns>
	bool GetCameraMoveEnd() { return easeCamera->GetEndFlag(); }
	/// <summary>
	/// �J�����̎擾
	/// </summary>
	/// <returns>�J����</returns>
	Camera* GetCamerapoint() { return camera_.get(); }
	/// <summary>
	/// �J�����A���O���̎擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetCameraAngle() { return angle_; }
	/// <summary>
	/// �v���C���[�̐ݒ�
	/// </summary>
	/// <param name="hunter">�v���C���[</param>
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	/// <summary>
	/// �C�[�W���O�f�[�^�̃��Z�b�g
	/// </summary>
	void EaseDataReset();

private: // �����o�ϐ�
	//�J����
	std::unique_ptr<Camera> camera_;
	// �C�[�W���O�̐i�s�x�p
	std::unique_ptr<EaseData> easeCamera;
	//�n���^�[
	Hunter* hunter_ = nullptr;
	// �p�x
	XMFLOAT2 angle_ = { 0.0f, 0.0f };
	// �����̃��Z�b�g�t���O
	bool cameraResetFlag = false;
};

