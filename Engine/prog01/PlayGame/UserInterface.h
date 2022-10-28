#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Sprite.h"
#include "Hunter.h"
#include "Monster.h"

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
	// �_���[�W���v�Z
	void DamageCalculate();
	// �X�^�~�i�v�Z
	void StrengthCalculate();
	// �A�C�e���֘A
	void ItemSelection();
	// �}�b�v�֌W
	void Map();
	// ���v�̌v�Z
	void ClockCalculate(int timer);

public: // �����o�֐�
	// �n���^�[��ݒ�
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// �n���^�[��ݒ�
	void SetMonster(Monster* monster) { monster_ = monster; }
	// �����Ă��邩�ǂ���
	bool GetIsPlayerDeath() { return isPlayerDeath_; }

private: // �����o�ϐ�
	//�n���^�[
	Hunter* hunter_ = nullptr;
	// �����X�^�[
	Monster* monster_ = nullptr;
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
	// ���v�̐j
	std::unique_ptr<Sprite> clockNeedle_;
	//�G�̗̑͂̃t���[��
	std::unique_ptr<Sprite> enemyLifeFrame_;
	//�G�̗̑͂̃Q�[�W
	std::unique_ptr<Sprite> enemyLifeGauge_;
	//�̗͂̐ԃQ�[�W
	std::unique_ptr<Sprite> enemyInnerLifeGauge_;
	// �A�C�e���̎��
	std::array<std::unique_ptr<Sprite>, 3> itemSprite_;
	// �ꌅ�̐���
	std::array<std::unique_ptr<Sprite>, 10> oneDigits_;
	// �\���̐���
	std::array<std::unique_ptr<Sprite>, 10> tenDigits_;
	// �A�C�e���̃t���[��
	std::unique_ptr<Sprite> itemFrame_;
	// �}�b�v
	std::unique_ptr<Sprite> mapSprite_;
	// �v���C���[�A�C�R��
	std::unique_ptr<Sprite> playerIcon_;
	// �G�l�~�[�A�C�R��
	std::unique_ptr<Sprite> enemyIcon_;
	// �U���̓A�b�v�̃A�C�R��
	std::unique_ptr<Sprite> attackIcon_;
	// �h��̓A�b�v�̃A�C�R��
	std::unique_ptr<Sprite> defenseIcon_;
	// LB�{�^���̃A�C�R��
	std::unique_ptr<Sprite> lbButtonIcon_;
	// X�{�^���̃A�C�R��
	std::unique_ptr<Sprite> xButtonIcon_;
	// B�{�^���̃A�C�R��
	std::unique_ptr<Sprite> bButtonIcon_;

	// �ꌅ
	int oneCount_ = 0;
	// �\��
	int tenCount_ = 0;

	//�C�[�W���O�̐i�s�x�p
	float hunterEaseTimer_ = 0.0f;
	float monsterEaseTimer_ = 0.0f;
	//�����X�^�[��HP
	float monsterHp_ = 0.0f;
	//�n���^�[��HP
	float hunterHp_ = 0.0f;
	//�n���^�[�̃X�^�~�i
	float hunterstamina_ = 0.0f;

	// �����Ă��邩�ǂ���
	bool isPlayerDeath_ = false;
	// �\���ڂ����邩
	bool isTenCountFlag_ = false;
};