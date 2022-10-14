#include "ItemManager.h"

// Ã“Iƒƒ“ƒo•Ï”‚ÌÀ‘Ì
const std::string ItemManager::baseDirectory = "Resources/csv/";

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::CsvLoad(std::string fName)
{
	std::ifstream ifs(baseDirectory + fName + ".csv");
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
			csvNumber.push_back(i);
		}
	}
}

int ItemManager::GetChipNum(int x, int y, std::string fName, int mapChipSize)
{
	const int X = x / mapChipSize;
	const int Y = y / mapChipSize;

	int i = Y * (int)Phase::Max + X;

	return csvNumber[i];
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
				itemData.push_back(tempData);
			}
		}
	}

}

void ItemManager::Finalize()
{
}

void ItemManager::Update()
{
}