#pragma once

#include <memory>

#include "ParticleManager.h"
#include "ObjParticle.h"

class ParticleEmitter
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	//

public: // �����o�֐�
	ParticleEmitter(ParticleManager* particleMan);
	ParticleEmitter(ObjParticle* objParticle);

	// �ǉ�
	void BubbleAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	void Add(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// ���W�̐ݒ�
	void SetCenter(const float mdPos) { mdPos_ = mdPos; }
	// ���x�̐ݒ�
	void SetVelocity(float mdVel) { mdVel_ = mdVel; }
	// �����x�̐ݒ�
	void SetAccel(float mdAcc) { mdAcc_ = mdAcc; }
	// �X�P�[���̏����l�̐ݒ�
	void SetStartScale(float startScale) { startScale_ = startScale; }
	void SetObjStartScale(XMFLOAT3 objStartScale) { objStartScale_ = objStartScale; }
	// �X�P�[���̏����l�̐ݒ�
	void SetEndScale(float endScale) { endScale_ = endScale; }
	void SetObjEndScale(XMFLOAT3 objEndScale) { objEndScale_ = objEndScale; }
	//�F(RGBA)�����l�̐ݒ�
	void SetStartColor(XMFLOAT4 s_color) { startColor_ = s_color; }
	//�F(RGBA)�ŏI�l�̐ݒ�
	void SetEndColor(XMFLOAT4 e_color) { endColor_ = e_color; }
	// �}�l�[�W���[�̐ݒ�
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	void SetObjParticle(ObjParticle* objParticle) { objParticle_ = objParticle; }

private: // �����o�ϐ�
	//���W
	XMFLOAT3 position_ = {};
	//���x
	XMFLOAT3 velocity_ = {};
	//�����x
	XMFLOAT3 accel_ = {};
	// �X�P�[�������l
	float startScale_ = 1.0f;
	XMFLOAT3 objStartScale_ = { 1.0f, 1.0f, 1.0f };
	// �X�P�[���ŏI�l
	float endScale_ = 0.0f;
	XMFLOAT3 objEndScale_ = { 0.0f, 0.0f, 0.0f };
	// �F(RGBA)�����l
	XMFLOAT4 startColor_ = { 1, 1, 1, 1};
	// �F(RGBA)�ŏI�l
	XMFLOAT4 endColor_ = {};
	//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
	float mdPos_ = 10.0f;
	//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
	float mdVel_ = 0.1f;
	//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
	float mdAcc_ = 0.001f;

	ParticleManager* particleMan_ = nullptr;
	ObjParticle* objParticle_ = nullptr;
};