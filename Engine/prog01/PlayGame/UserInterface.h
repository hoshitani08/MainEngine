#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Sprite.h"
#include "Hunter.h"
#include "Monster.h"

class UserInterface
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // メンバ関数
	// コンストラクタ
	UserInterface();
	// デストラクタ
	~UserInterface();
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 奥の描画
	void BackDraw();
	// 手前の描画
	void NearDraw();
	// HP減少時のイージング
	void HpEase();
	// ダメージを計算
	void DamageCalculate();
	// スタミナ計算
	void StrengthCalculate();
	// アイテム関連
	void ItemSelection();
	// マップ関係
	void Map();
	// 時計の計算
	void ClockCalculate(int timer);

public: // メンバ関数
	// ハンターを設定
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// ハンターを設定
	void SetMonster(Monster* monster) { monster_ = monster; }
	// 生きているかどうか
	bool GetIsPlayerDeath() { return isPlayerDeath_; }

private: // メンバ変数
	//ハンター
	Hunter* hunter_ = nullptr;
	// モンスター
	Monster* monster_ = nullptr;
	//スタミナのフレーム
	std::unique_ptr<Sprite> strengthFrame_;
	//体力のフレーム
	std::unique_ptr<Sprite> lifeFrame_;
	//スタミナのゲージ
	std::unique_ptr<Sprite> strengthGauge_;
	//体力のゲージ
	std::unique_ptr<Sprite> lifeGauge_;
	//体力の赤ゲージ
	std::unique_ptr<Sprite> innerLifeGauge_;
	//時計のフレーム
	std::unique_ptr<Sprite> clockFrame_;
	// 時計の針
	std::unique_ptr<Sprite> clockNeedle_;
	//敵の体力のフレーム
	std::unique_ptr<Sprite> enemyLifeFrame_;
	//敵の体力のゲージ
	std::unique_ptr<Sprite> enemyLifeGauge_;
	//体力の赤ゲージ
	std::unique_ptr<Sprite> enemyInnerLifeGauge_;
	// アイテムの種類
	std::array<std::unique_ptr<Sprite>, 3> itemSprite_;
	// 一桁の数字
	std::array<std::unique_ptr<Sprite>, 10> oneDigits_;
	// 十桁の数字
	std::array<std::unique_ptr<Sprite>, 10> tenDigits_;
	// アイテムのフレーム
	std::unique_ptr<Sprite> itemFrame_;
	// マップ
	std::unique_ptr<Sprite> mapSprite_;
	// プレイヤーアイコン
	std::unique_ptr<Sprite> playerIcon_;
	// エネミーアイコン
	std::unique_ptr<Sprite> enemyIcon_;
	// 攻撃力アップのアイコン
	std::unique_ptr<Sprite> attackIcon_;
	// 防御力アップのアイコン
	std::unique_ptr<Sprite> defenseIcon_;
	// LBボタンのアイコン
	std::unique_ptr<Sprite> lbButtonIcon_;
	// Xボタンのアイコン
	std::unique_ptr<Sprite> xButtonIcon_;
	// Bボタンのアイコン
	std::unique_ptr<Sprite> bButtonIcon_;

	// 一桁
	int oneCount_ = 0;
	// 十桁
	int tenCount_ = 0;

	//イージングの進行度用
	float hunterEaseTimer_ = 0.0f;
	float monsterEaseTimer_ = 0.0f;
	//モンスターのHP
	float monsterHp_ = 0.0f;
	//ハンターのHP
	float hunterHp_ = 0.0f;
	//ハンターのスタミナ
	float hunterstamina_ = 0.0f;

	// 生きているかどうか
	bool isPlayerDeath_ = false;
	// 十桁目があるか
	bool isTenCountFlag_ = false;
};