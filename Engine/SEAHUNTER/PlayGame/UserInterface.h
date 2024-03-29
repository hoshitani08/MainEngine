#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>

#include "Sprite.h"
#include "Hunter.h"
#include "Monster.h"
#include "Ease.h"

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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UserInterface(Hunter* hunter, Monster* monster);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UserInterface();
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
	/// 奥の描画
	/// </summary>
	void BackDraw();
	/// <summary>
	/// 手前の描画
	/// </summary>
	void NearDraw();
	/// <summary>
	/// HP減少時のイージング
	/// </summary>
	void HpEase();
	/// <summary>
	/// ダメージを計算
	/// </summary>
	void DamageCalculate();
	/// <summary>
	/// スタミナ計算
	/// </summary>
	void StrengthCalculate();
	/// <summary>
	/// アイテム関連
	/// </summary>
	void ItemSelection();
	/// <summary>
	/// マップ関係
	/// </summary>
	void Map();
	/// <summary>
	/// 時計の計算
	/// </summary>
	/// <param name="timer">秒</param>
	void ClockCalculate(int timer);

public: // メンバ関数
	/// <summary>
	/// 生存フラグの取得
	/// </summary>
	/// <returns>生存フラグ</returns>
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
	// アイテムの背景
	std::unique_ptr<Sprite> itemBackground_;
	// アイテムの選択時の背景
	std::unique_ptr<Sprite> itemSBackground_;
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
	// 敵の名前
	std::unique_ptr<Sprite> enemyText_;
	// フレームの背景
	std::array<std::unique_ptr<Sprite>, 3> frameBackground_;
	// モンスター用のイージングの進行度用
	std::unique_ptr<EaseData> monsterEase_;
	// ハンター用のイージングの進行度用
	std::unique_ptr<EaseData> hunterEase_;
	// アイテム用のイージングの進行度用
	std::unique_ptr<EaseData> itemEase_;
	// フレーム用のイージングの進行度用
	std::unique_ptr<EaseData> frameEase_;
	// フレーム用のイージングの進行度用
	std::unique_ptr<EaseData> frameEase2_;
	// 選択時用のイージングの進行度用
	std::unique_ptr<EaseData> selectionEase_;
	// ボタン用のイージングの進行度用
	std::unique_ptr<EaseData> buttonEase_;

	// 一桁
	int oneCount_ = 0;
	// 十桁
	int tenCount_ = 0;

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
	// ボタンのリセットフラグ
	bool buttonReset_ = false;
};