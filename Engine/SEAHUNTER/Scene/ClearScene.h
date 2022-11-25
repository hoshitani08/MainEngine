#pragma once

#include <memory>
#include <array>
#include <algorithm>

#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"
#include "SceneChange.h"

class ClearScene : public BaseScene
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

public: // �����o�֐�
	// �f�X�g���N�^
	~ClearScene();
	// ������
	void Initialize() override;
	//�I��
	void Finalize() override;
	// ���t���[������
	void Update() override;
	// �`��
	void Draw() override;
	// �G�t�F�N�g�`��
	void EffectDraw() override;

	// �{�^���֌W
	void Select();
	void Shake();
	// �C�[�W���O
	void EaseMove();

private: // �����o�ϐ�
	// �J����
	std::unique_ptr<Camera> camera_;
	// ���C�g
	std::unique_ptr<LightGroup> light_;
	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> clearTile_;
	std::unique_ptr<Object3d> continueTile_;
	std::unique_ptr<Object3d> quitTile_;
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;
	// ����t���O
	bool determinationFlag_ = true;
	// �V�F�C�N���Ă��邩
	bool isShake_ = false;
	// �V�F�C�N�^�C�}�[
	int shakeTimer_ = 0;
	// �����l
	int attenuation_ = 0;
	// �V�F�C�N�O�̈ʒu
	XMFLOAT3 savePos_ = {};
	// �A�̗ʂ̒����p�^�C�}�[
	int bubbleTimer_ = 100;
	// �C�[�W���O�p�ʒu
	std::array<XMFLOAT3, 3> startPosition_ = {};
	std::array<XMFLOAT3, 3> endPosition_ = {};
	// �C�[�W���O�̐i�s�x�p
	float easeTimer_ = 0.0f;
	// �C�[�W���O���I�������
	bool isEaseFlag_ = false;
};