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
	static const string baseDirectory;

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

private: // �����o�֐�
	ItemManager();
	~ItemManager();

	//CSV�t�@�C���ǂݍ���
	void CsvLoad(std::string fName);
	//�}�b�v�i���o�[�w��
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);

public: // �����o�֐�
	ItemManager(const ItemManager& r) = delete;
	ItemManager& operator=(const ItemManager& r) = delete;

	static ItemManager* GetInstance();
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();

public: // �����o�֐�
	// �A�C�e���̎�ނ̎擾
	ItemType GetItemType(int count) { return itemData[count].name; }
	// �A�C�e���̌��̎擾
	int GetItemQuantity(int count) { return itemData[count].quantity; }
	// �A�C�e���̌��̐ݒ�
	void SetItemQuantity(int count, int quantity) { itemData[count].quantity = quantity; }

private: // �����o�ϐ�
	std::vector<int> csvNumber;
	std::vector<ItemData> itemData;
};

