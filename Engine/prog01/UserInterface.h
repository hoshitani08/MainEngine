#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Sprite.h"

class UserInterface
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
	//�R���X�g���N�^
	UserInterface();
	//�f�X�g���N�^
	~UserInterface();
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// ���̕`��
	void BackDraw();
	// ��O�̕`��
	void NearDraw();

	void HpEase();

private: // �����o�ϐ�
	//�X�^�~�i�̃t���[��
	std::unique_ptr<Sprite> strengthFrame_;
	//�̗͂̃t���[��
	std::unique_ptr<Sprite> lifeFrame_;
	//�X�^�~�i�̃Q�[�W
	std::unique_ptr<Sprite> strengthGauge_;
	//�̗͂̃Q�[�W
	std::unique_ptr<Sprite> lifeGauge_;
	//�̗͂̐ԃQ�[�W
	std::unique_ptr<Sprite> innerLifeGauge_;
	//���v�̃t���[��
	std::unique_ptr<Sprite> clockFrame_;

	//�C�[�W���O�̐i�s�x�p
	float easeTimer = 0.0f;

	XMFLOAT2 hpGaugeSize = {};
	XMFLOAT2 hpGaugeDamageSize = {};
};