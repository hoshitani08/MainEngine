#include "ItemManager.h"

// 静的メンバ変数の実体
const std::string ItemManager::BASE_DIRECTORY = "Resources/csv/";

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::CsvLoad(const std::string& fName)
{
	std::ifstream ifs(BASE_DIRECTORY + fName + ".csv");
	std::string line;

	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(stoi(field));
		}
		for (auto i : result)
		{
			csvNumber_.push_back(i);
		}
	}
}

int ItemManager::GetChipNum(int x, int y, const std::string& fName, int mapChipSize)
{
	const int X = x / mapChipSize;
	const int Y = y / mapChipSize;

	int i = Y * static_cast<int>(Phase::Max) + X;

	return csvNumber_[i];
}

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Initialize()
{
	CsvLoad("item");

	// アイテムの読み込み
	for (int i = 0; i < static_cast<int>(ItemType::Max); i++)
	{
		ItemData tempData;
		for (int j = 0; j < static_cast<int>(Phase::Max); j++)
		{
			if (GetChipNum(j, i, "item") == static_cast<int>(ItemType::Healing) && j == 0)
			{
				tempData.name = ItemType::Healing;
			}
			else if (GetChipNum(j, i, "item") == static_cast<int>(ItemType::AttackBuff) && j == 0)
			{
				tempData.name = ItemType::AttackBuff;
			}
			else if (GetChipNum(j, i, "item") == static_cast<int>(ItemType::DefenseBuff) && j == 0)
			{
				tempData.name = ItemType::DefenseBuff;
			}

			if (j != 0)
			{
				tempData.quantity = GetChipNum(j, i, "item");
				tempData.maxCount = tempData.quantity;
				itemData_.push_back(tempData);
			}
		}
	}

}

void ItemManager::Finalize()
{
	itemData_.clear();
	defense_ = {};
	attack_ = {};
}

void ItemManager::Update()
{
}

void ItemManager::BuffUpdate()
{
	// 防御バフの時間管理
	if (defense_.isBuff)
	{
		defense_.buffTimer++;

		if (defense_.buffTimer >= 60)
		{
			defense_.second++;
			defense_.buffTimer = 0;
		}

		if (defense_.second >= 120)
		{
			defense_.isBuff = false;
			defense_.second = 0;
			defense_.buffTimer = 0;
			defense_.magnification = 1.0f;
		}
	}

	// 攻撃バフの時間管理
	if (attack_.isBuff)
	{
		attack_.buffTimer++;

		if (attack_.buffTimer >= 60)
		{
			attack_.second++;
			attack_.buffTimer = 0;
		}

		if (attack_.second >= 120)
		{
			attack_.isBuff = false;
			attack_.second = 0;
			attack_.buffTimer = 0;
			attack_.magnification = 1.0f;
		}
	}
}

void ItemManager::SetDefenseBuff(bool isBuff, int buffTimer, int second, float magnification)
{
	defense_.isBuff = isBuff;
	defense_.buffTimer = buffTimer;
	defense_.second = second;
	defense_.magnification = magnification;
}

void ItemManager::SetAttackBuff(bool isBuff, int buffTimer, int second, float magnification)
{
	attack_.isBuff = isBuff;
	attack_.buffTimer = buffTimer;
	attack_.second = second;
	attack_.magnification = magnification;
}
