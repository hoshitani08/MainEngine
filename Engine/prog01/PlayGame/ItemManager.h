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

	struct BuffData
	{
		bool isBuff = false;
		int buffTimer = 0;
		int second = 0;
		float magnification = 1.0f;
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
	// バフの更新
	void BuffUpdate();

public: // メンバ関数
	// アイテムの種類の取得
	ItemType GetItemType(int count) { return itemData[count].name; }
	// アイテムの個数の取得
	int GetItemQuantity(int count) { return itemData[count].quantity; }
	// アイテムの個数の設定
	void SetItemQuantity(int count, int quantity) { itemData[count].quantity = quantity; }
	// 防御バフの設定
	void SetDefenseBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	// 攻撃バフの設定
	void SetAttackBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	// 防御バフ倍率
	float DefenseBuffMagnification() { return defense_.magnification; }
	// 攻撃バフ倍率
	float AttackBuffMagnification() { return attack_.magnification; }
	// 攻撃バフがかかっているか
	bool IsAttackBuff() { return attack_.isBuff; }
	// 防御バフがかかっているか
	bool IsDefenseBuff() { return defense_.isBuff; }
	// 攻撃バフタイマーの取得
	int GetAttackBuffSecondTimer() { return attack_.second; }
	int GetAttackBuffTimer() { return attack_.buffTimer; }
	// 防御バフタイマーの取得
	int GetDefenseBuffSecondTimer() { return defense_.second; }
	int GetDefenseBuffTimer() { return defense_.buffTimer; }

private: // メンバ変数
	std::vector<int> csvNumber;
	// アイテムのデータ
	std::vector<ItemData> itemData;
	// 防御力の上昇
	BuffData defense_;
	// 攻撃力の上昇
	BuffData attack_;
};

