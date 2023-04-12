#include "ItemManager.h"

// 静的メンバ変数の実体
const std::string ItemManager::BASE_DIRECTORY = "Resources/csv/";

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;
	return &instance;
}

void ItemManager::Initialize()
{
	// レベルデータの読み込み
	std::unique_ptr<LevelData> levelData = LevelLoader::LoadFile("itemData");

	for (auto& itemData : levelData->items)
	{
		ItemData tempData;

		if (itemData.fileName == "Healing")
		{
			tempData.name = ItemType::Healing;
		}
		else if(itemData.fileName == "AttackBuff")
		{
			tempData.name = ItemType::AttackBuff;
		}
		else if (itemData.fileName == "Healing")
		{
			tempData.name = ItemType::DefenseBuff;
		}

		tempData.quantity = itemData.quantity;
		tempData.maxCount = itemData.quantity;
		itemData_.push_back(tempData);
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
