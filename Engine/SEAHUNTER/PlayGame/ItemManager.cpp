#include "ItemManager.h"

// ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃ
const std::string ItemManager::BASE_DIRECTORY = "Resources/csv/";

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::CsvLoad(std::string fName)
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

int ItemManager::GetChipNum(int x, int y, std::string fName, int mapChipSize)
{
	const int X = x / mapChipSize;
	const int Y = y / mapChipSize;

	int i = Y * (int)Phase::Max + X;

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

	
	for (int i = 0; i < (int)ItemType::Max; i++)
	{
		ItemData tempData;
		for (int j = 0; j < (int)Phase::Max; j++)
		{
			if (GetChipNum(j, i, "item") == (int)ItemType::Healing && j == 0)
			{
				tempData.name = ItemType::Healing;
			}
			else if (GetChipNum(j, i, "item") == (int)ItemType::AttackBuff && j == 0)
			{
				tempData.name = ItemType::AttackBuff;
			}
			else if (GetChipNum(j, i, "item") == (int)ItemType::DefenseBuff && j == 0)
			{
				tempData.name = ItemType::DefenseBuff;
			}

			if (j != 0)
			{
				tempData.quantity = GetChipNum(j, i, "item");
				itemData_.push_back(tempData);
			}
		}
	}

}

void ItemManager::Finalize()
{
	itemData_.clear();
}

void ItemManager::Update()
{
}

void ItemManager::BuffUpdate()
{
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
