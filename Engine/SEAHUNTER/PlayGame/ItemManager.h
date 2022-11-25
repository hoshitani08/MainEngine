#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

class ItemManager
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	// std::���ȗ�
	using string = std::string;

public:
	// ���[�g�p�X
	static const string BASE_DIRECTORY;

public: // �T�u�N���X
	enum class ItemType
	{
		Healing,
		AttackBuff,
		DefenseBuff,
		Max,
	};

	enum class Phase
	{
		ItemType,
		Quantity,
		Max,
	};

	struct ItemData
	{
		ItemType name = ItemType::Max;
		int quantity = 0;
	};

	struct BuffData
	{
		bool isBuff = false;
		int buffTimer = 0;
		int second = 0;
		float magnification = 1.0f;
	};

private: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ItemManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ItemManager();
	/// <summary>
	/// CSV�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	void CsvLoad(std::string fName);
	/// <summary>
	/// �}�b�v�i���o�[�w��
	/// </summary>
	/// <param name="x">�ő�̉���</param>
	/// <param name="y">�ő�̏c��</param>
	/// <param name="fName">�t�@�C����</param>
	/// <param name="mapChipSize"></param>
	/// <returns></returns>
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);

public: // �����o�֐�
	ItemManager(const ItemManager& r) = delete;
	ItemManager& operator=(const ItemManager& r) = delete;
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static ItemManager* GetInstance();
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
	/// �o�t�̍X�V
	/// </summary>
	void BuffUpdate();

public: // �����o�֐�
	/// <summary>
	/// �A�C�e���̖��O�̎擾
	/// </summary>
	/// <param name="count">�ԍ�</param>
	/// <returns></returns>
	ItemType GetItemType(int count) { return itemData_[count].name; }
	/// <summary>
	/// �A�C�e���̌��̎擾
	/// </summary>
	/// <param name="count">�ԍ�</param>
	/// <returns></returns>
	int GetItemQuantity(int count) { return itemData_[count].quantity; }
	/// <summary>
	/// �A�C�e���̌��̐ݒ�
	/// </summary>
	/// <param name="count">�ԍ�</param>
	/// <param name="quantity">��</param>
	void SetItemQuantity(int count, int quantity) { itemData_[count].quantity = quantity; }
	/// <summary>
	/// �h��o�t�̐ݒ�
	/// </summary>
	/// <param name="isBuff">�h��o�t�������邩</param>
	/// <param name="buffTimer">�J�n���̃t���[��</param>
	/// <param name="second">�J�n���̕b</param>
	/// <param name="magnification">�{��</param>
	void SetDefenseBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	/// <summary>
	/// �U���o�t�̐ݒ�
	/// </summary>
	/// <param name="isBuff">�U���o�t�������邩</param>
	/// <param name="buffTimer">�J�n���̃t���[��</param>
	/// <param name="second">�J�n���̕b</param>
	/// <param name="magnification">�{��</param>
	void SetAttackBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	/// <summary>
	/// �h��o�t�{���̎擾
	/// </summary>
	/// <returns>�h��o�t�{��</returns>
	float DefenseBuffMagnification() { return defense_.magnification; }
	/// <summary>
	/// �U���o�t�{���̎擾
	/// </summary>
	/// <returns>�U���o�t�{��</returns>
	float AttackBuffMagnification() { return attack_.magnification; }
	/// <summary>
	/// �U���o�t�t���O�̎擾
	/// </summary>
	/// <returns>�U���o�t�t���O</returns>
	bool IsAttackBuff() { return attack_.isBuff; }
	/// <summary>
	/// �h��o�t�t���O�̎擾
	/// </summary>
	/// <returns>�h��o�t�t���O</returns>
	bool IsDefenseBuff() { return defense_.isBuff; }
	/// <summary>
	/// �U���o�t�^�C�}�[(�b)�̎擾
	/// </summary>
	/// <returns>�^�C�}�[(�b)</returns>
	int GetAttackBuffSecondTimer() { return attack_.second; }
	/// <summary>
	/// �U���o�t�^�C�}�[(�t���[��)�̎擾
	/// </summary>
	/// <returns>�^�C�}�[(�t���[��)</returns>
	int GetAttackBuffTimer() { return attack_.buffTimer; }
	/// <summary>
	/// �h��o�t�^�C�}�[(�b)�̎擾
	/// </summary>
	/// <returns>�^�C�}�[(�b)</returns>
	int GetDefenseBuffSecondTimer() { return defense_.second; }
	/// <summary>
	/// �h��o�t�^�C�}�[(�t���[��)�̎擾
	/// </summary>
	/// <returns>�^�C�}�[(�t���[��)</returns>
	int GetDefenseBuffTimer() { return defense_.buffTimer; }

private: // �����o�ϐ�
	std::vector<int> csvNumber_;
	// �A�C�e���̃f�[�^
	std::vector<ItemData> itemData_;
	// �h��͂̏㏸
	BuffData defense_;
	// �U���͂̏㏸
	BuffData attack_;
};

