#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"
#include "LightGroup.h"

#include "Sprite.h"
#include "DebugText.h"
#include "Object3d.h"

#include "ParticleEmitter.h"

#include <memory>
#include <array>
#include <algorithm>

class TitleScene : public BaseScene
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
	~TitleScene();
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

	//�{�^���֌W
	void Select();
	void Shake();

private: // �����o�ϐ�
	//�J����
	std::unique_ptr<Camera> camera_;
	//���C�g
	std::unique_ptr<LightGroup> light_;
	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleMan_;
	std::unique_ptr<ParticleEmitter> bubble_;
	//�I�u�W�F�N�g
	std::unique_ptr<Object3d> titleTile_;
	std::unique_ptr<Object3d> startTile_;
	std::unique_ptr<Object3d> escapeTile_;
	// ����t���O
	bool determinationFlag = true;
	// �V�F�C�N���Ă��邩
	bool isShake = false;
	// �V�F�C�N�^�C�}�[
	int shakeTimer = 0;
	// �����l
	int attenuation = 0;
	// �V�F�C�N�O�̈ʒu
	XMFLOAT3 savePos = {};
	// �A�̗ʂ̒����p�^�C�}�[
	int bubbleTimer_ = 100;
};