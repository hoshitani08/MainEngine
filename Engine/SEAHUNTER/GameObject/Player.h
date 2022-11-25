#pragma once

#include "Object3d.h"
#include "ParticleManager.h"
#include "Input.h"

class Player : public Object3d
{
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(Model* model = nullptr);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;
	// �}�l�[�W���[�̐ݒ�
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	void SetInput(Input* input) { input_ = input; }

private: // �����o�ϐ�
	ParticleManager* particleMan_ = nullptr;
	Input* input_ = nullptr;
	//�ڒn�t���O
	bool onGround_ = true;
	// �����x�N�g��
	DirectX::XMVECTOR fallV_;
};