#pragma once

class SceneManager;

class BaseScene
{
public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// エフェクトあり描画
	/// </summary>
	virtual void EffectDraw() = 0;
};