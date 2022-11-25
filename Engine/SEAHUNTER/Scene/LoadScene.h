#pragma once

#include <memory>
#include <array>
#include <algorithm>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "Sprite.h"

class LoadScene : public BaseScene
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
	/// デストラクタ
	/// </summary>
	~LoadScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// エフェクトあり描画
	/// </summary>
	void EffectDraw() override;

private: // メンバ変数
	// 背景
	std::unique_ptr<Sprite> black_;
	// 
	std::unique_ptr<Sprite> sphere_;
	//
	std::unique_ptr<Sprite> loading_;
	//
	float angle = 0.0f;
	//
	int timer_ = 0;
};

