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
	static const string BASE_DIRECTORY;

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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemManager();
	/// <summary>
	/// CSVファイル読み込み
	/// </summary>
	/// <param name="fName">ファイル名</param>
	void CsvLoad(std::string fName);
	/// <summary>
	/// マップナンバー指定
	/// </summary>
	/// <param name="x">最大の横幅</param>
	/// <param name="y">最大の縦幅</param>
	/// <param name="fName">ファイル名</param>
	/// <param name="mapChipSize"></param>
	/// <returns></returns>
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);

public: // メンバ関数
	ItemManager(const ItemManager& r) = delete;
	ItemManager& operator=(const ItemManager& r) = delete;
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ItemManager* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// バフの更新
	/// </summary>
	void BuffUpdate();

public: // メンバ関数
	/// <summary>
	/// アイテムの名前の取得
	/// </summary>
	/// <param name="count">番号</param>
	/// <returns></returns>
	ItemType GetItemType(int count) { return itemData_[count].name; }
	/// <summary>
	/// アイテムの個数の取得
	/// </summary>
	/// <param name="count">番号</param>
	/// <returns></returns>
	int GetItemQuantity(int count) { return itemData_[count].quantity; }
	/// <summary>
	/// アイテムの個数の設定
	/// </summary>
	/// <param name="count">番号</param>
	/// <param name="quantity">数</param>
	void SetItemQuantity(int count, int quantity) { itemData_[count].quantity = quantity; }
	/// <summary>
	/// 防御バフの設定
	/// </summary>
	/// <param name="isBuff">防御バフをかけるか</param>
	/// <param name="buffTimer">開始時のフレーム</param>
	/// <param name="second">開始時の秒</param>
	/// <param name="magnification">倍率</param>
	void SetDefenseBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	/// <summary>
	/// 攻撃バフの設定
	/// </summary>
	/// <param name="isBuff">攻撃バフをかけるか</param>
	/// <param name="buffTimer">開始時のフレーム</param>
	/// <param name="second">開始時の秒</param>
	/// <param name="magnification">倍率</param>
	void SetAttackBuff(bool isBuff, int buffTimer = 0, int second = 0, float magnification = 1.0f);
	/// <summary>
	/// 防御バフ倍率の取得
	/// </summary>
	/// <returns>防御バフ倍率</returns>
	float DefenseBuffMagnification() { return defense_.magnification; }
	/// <summary>
	/// 攻撃バフ倍率の取得
	/// </summary>
	/// <returns>攻撃バフ倍率</returns>
	float AttackBuffMagnification() { return attack_.magnification; }
	/// <summary>
	/// 攻撃バフフラグの取得
	/// </summary>
	/// <returns>攻撃バフフラグ</returns>
	bool IsAttackBuff() { return attack_.isBuff; }
	/// <summary>
	/// 防御バフフラグの取得
	/// </summary>
	/// <returns>防御バフフラグ</returns>
	bool IsDefenseBuff() { return defense_.isBuff; }
	/// <summary>
	/// 攻撃バフタイマー(秒)の取得
	/// </summary>
	/// <returns>タイマー(秒)</returns>
	int GetAttackBuffSecondTimer() { return attack_.second; }
	/// <summary>
	/// 攻撃バフタイマー(フレーム)の取得
	/// </summary>
	/// <returns>タイマー(フレーム)</returns>
	int GetAttackBuffTimer() { return attack_.buffTimer; }
	/// <summary>
	/// 防御バフタイマー(秒)の取得
	/// </summary>
	/// <returns>タイマー(秒)</returns>
	int GetDefenseBuffSecondTimer() { return defense_.second; }
	/// <summary>
	/// 防御バフタイマー(フレーム)の取得
	/// </summary>
	/// <returns>タイマー(フレーム)</returns>
	int GetDefenseBuffTimer() { return defense_.buffTimer; }

private: // メンバ変数
	std::vector<int> csvNumber_;
	// アイテムのデータ
	std::vector<ItemData> itemData_;
	// 防御力の上昇
	BuffData defense_;
	// 攻撃力の上昇
	BuffData attack_;
};

