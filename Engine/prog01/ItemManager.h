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
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	// std::を省略
	using string = std::string;

public:
	// ルートパス
	static const string baseDirectory;

public: // サブクラス
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

private: // メンバ関数
	ItemManager();
	~ItemManager();

	//CSVファイル読み込み
	void CsvLoad(std::string fName);
	//マップナンバー指定
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);

public: // メンバ関数
	ItemManager(const ItemManager& r) = delete;
	ItemManager& operator=(const ItemManager& r) = delete;

	static ItemManager* GetInstance();
	// 初期化
	void Initialize();
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();

public: // メンバ関数
	// アイテムの種類の取得
	ItemType GetItemType(int count) { return itemData[count].name; }
	// アイテムの個数の取得
	int GetItemQuantity(int count) { return itemData[count].quantity; }
	// アイテムの個数の設定
	void SetItemQuantity(int count, int quantity) { itemData[count].quantity = quantity; }

private: // メンバ変数
	std::vector<int> csvNumber;
	std::vector<ItemData> itemData;
};

