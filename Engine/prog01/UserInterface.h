#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Sprite.h"
#include "Hunter.h"

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
	// ハンターを設定
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	// ダメージを計算
	void DamageCalculate();
	// スタミナ計算
	void StrengthCalculate();
	// 生きているかどうか
	bool GetIsDeath() { return isDeath_; }

private: // メンバ変数
	//ハンター
	Hunter* hunter_ = nullptr;
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
	//イージングの進行度用
	float easeTimer_ = 0.0f;
	// 生きているかどうか
	bool isDeath_ = false;
};