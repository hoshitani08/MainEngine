#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Sprite.h"
#include "Hunter.h"
#include "Monster.h"
#include "Ease.h"

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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UserInterface(Hunter* hunter, Monster* monster);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UserInterface();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// ���̕`��
	/// </summary>
	void BackDraw();
	/// <summary>
	/// ��O�̕`��
	/// </summary>
	void NearDraw();
	/// <summary>
	/// HP�������̃C�[�W���O
	/// </summary>
	void HpEase();
	/// <summary>
	/// �_���[�W���v�Z
	/// </summary>
	void DamageCalculate();
	/// <summary>
	/// �X�^�~�i�v�Z
	/// </summary>
	void StrengthCalculate();
	/// <summary>
	/// �A�C�e���֘A
	/// </summary>
	void ItemSelection();
	/// <summary>
	/// �}�b�v�֌W
	/// </summary>
	void Map();
	/// <summary>
	/// ���v�̌v�Z
	/// </summary>
	/// <param name="timer">�b</param>
	void ClockCalculate(int timer);

public: // �����o�֐�
	/// <summary>
	/// �����t���O�̎擾
	/// </summary>
	/// <returns>�����t���O</returns>
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
	// �A�C�e���̔w�i
	std::unique_ptr<Sprite> itemBackground_;
	// �A�C�e���̑I�����̔w�i
	std::unique_ptr<Sprite> itemSBackground_;
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
	// �G�̖��O
	std::unique_ptr<Sprite> enemyText_;
	// �t���[���̔w�i
	std::array<std::unique_ptr<Sprite>, 3> frameBackground_;
	// �����X�^�[�p�̃C�[�W���O�̐i�s�x�p
	std::unique_ptr<EaseData> monsterEase_;
	// �n���^�[�p�̃C�[�W���O�̐i�s�x�p
	std::unique_ptr<EaseData> hunterEase_;
	// �A�C�e���p�̃C�[�W���O�̐i�s�x�p
	std::unique_ptr<EaseData> itemEase_;
	// �A�C�e���̃t���[���p�̃C�[�W���O�̐i�s�x�p
	std::unique_ptr<EaseData> frameEase_;
	std::unique_ptr<EaseData> frameEase2_;
	std::unique_ptr<EaseData> selectionEase_;
	std::unique_ptr<EaseData> buttonEase_;

	// �ꌅ
	int oneCount_ = 0;
	// �\��
	int tenCount_ = 0;

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
	//
	bool buttonReset = false;
};