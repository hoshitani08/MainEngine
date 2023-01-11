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

	/// <summary>
	/// �ǉ�(�A)
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void BubbleAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// �ǉ�(���p)
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void BloodAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// �ǉ�(�A�C�e���p)
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void ItemAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// �ǉ�(�񕜗p)
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void HealAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr, float size = 0.5f);
	/// <summary>
	/// �ǉ�(����)
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void SandAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// �ǉ�
	/// </summary>
	/// <param name="count">��</param>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="model">���f��</param>
	void Add(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="mdPos">���W</param>
	void SetCenter(const float mdPos) { mdPos_ = mdPos; }
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="mdPos">���W</param>
	void SetCenter(const XMFLOAT3 objMdPos) { objMdPos_ = objMdPos; }
	/// <summary>
	/// ���x�̐ݒ�
	/// </summary>
	/// <param name="mdVel">���x</param>
	void SetVelocity(float mdVel) { mdVel_ = mdVel; }
	/// <summary>
	/// �����x�̐ݒ�
	/// </summary>
	/// <param name="mdAcc">�����x</param>
	void SetAccel(float mdAcc) { mdAcc_ = mdAcc; }
	/// <summary>
	/// �X�P�[���̏����l�̐ݒ�
	/// </summary>
	/// <param name="startScale">�X�P�[���̏����l</param>
	void SetStartScale(float startScale) { startScale_ = startScale; }
	/// <summary>
	/// �X�P�[���̏����l�̐ݒ�
	/// </summary>
	/// <param name="startScale">�X�P�[���̏����l</param>
	void SetObjStartScale(XMFLOAT3 objStartScale) { objStartScale_ = objStartScale; }
	/// <summary>
	/// �X�P�[���̏����l�̐ݒ�
	/// </summary>
	/// <param name="endScale">�X�P�[���̏����l</param>
	void SetEndScale(float endScale) { endScale_ = endScale; }
	/// <summary>
	/// �X�P�[���̏����l�̐ݒ�
	/// </summary>
	/// <param name="endScale">�X�P�[���̏����l</param>
	void SetObjEndScale(XMFLOAT3 objEndScale) { objEndScale_ = objEndScale; }
	/// <summary>
	/// �F(RGBA)�����l�̐ݒ�
	/// </summary>
	/// <param name="s_color">�F(RGBA)�����l</param>
	void SetStartColor(XMFLOAT4 s_color) { startColor_ = s_color; }
	/// <summary>
	/// �F(RGBA)�ŏI�l�̐ݒ�
	/// </summary>
	/// <param name="e_color">�F(RGBA)�ŏI�l</param>
	void SetEndColor(XMFLOAT4 e_color) { endColor_ = e_color; }
	/// <summary>
	/// �}�l�[�W���[�̐ݒ�
	/// </summary>
	/// <param name="particleMan">�}�l�[�W���[</param>
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	/// <summary>
	/// �}�l�[�W���[�̐ݒ�
	/// </summary>
	/// <param name="particleMan">�}�l�[�W���[</param>
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
	XMFLOAT4 startColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// �F(RGBA)�ŏI�l
	XMFLOAT4 endColor_ = {};
	//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
	float mdPos_ = 10.0f;
	XMFLOAT3 objMdPos_ = { 10.0f, 10.0f, 10.0f };
	//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
	float mdVel_ = 0.1f;
	//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
	float mdAcc_ = 0.001f;

	ParticleManager* particleMan_ = nullptr;
	ObjParticle* objParticle_ = nullptr;
};