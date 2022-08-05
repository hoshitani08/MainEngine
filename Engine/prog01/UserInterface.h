#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Sprite.h"
#include "Hunter.h"

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
	// �R���X�g���N�^
	UserInterface();
	// �f�X�g���N�^
	~UserInterface();
	// ������
	void Initialize();
	// �I��
	void Finalize();
	// ���t���[������
	void Update();
	// ���̕`��
	void BackDraw();
	// ��O�̕`��
	void NearDraw();
	// HP�������̃C�[�W���O
	void HpEase();
	// �n���^�[��ݒ�
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// �_���[�W���v�Z
	void DamageCalculate();
	// �X�^�~�i�v�Z
	void StrengthCalculate();
	// �����Ă��邩�ǂ���
	bool GetIsDeath() { return isDeath_; }

private: // �����o�ϐ�
	//�n���^�[
	Hunter* hunter_ = nullptr;
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
	float easeTimer_ = 0.0f;
	// �����Ă��邩�ǂ���
	bool isDeath_ = false;
};