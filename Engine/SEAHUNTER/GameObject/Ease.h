#pragma once

#include <DirectXMath.h>
#include <iostream>
#include <array>
#include <algorithm>
#include <functional>

enum class EaseType
{
	In,
	Out,
	InOut,
};

enum class EaseFunctionType
{
	Quad,
	Cubic,
	Quart,
	Quint,
	Sine,
	Expo,
	Circ,
	Back,
	SoftBack,
	Elastic,
	Bounce,
	Linear,
};

class Ease
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // 定数
	static const float PI;

public:
	static const float Action(EaseType easeType, EaseFunctionType easeFunctionType, const float& start, const float& end, const float t);
	static const XMFLOAT2 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT4& start, const XMFLOAT4& end, const float t);

private:
	/// <summary>
	/// クワッド
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Quad(float t);
	/// <summary>
	/// キュービック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Cubic(float t);
	/// <summary>
	/// クォート
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Quart(float t);
	/// <summary>
	/// クイント
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Quint(float t);
	/// <summary>
	/// サイン
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Sine(float t);
	/// <summary>
	/// エキスポ
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Expo(float t);
	/// <summary>
	/// サークル
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Circ(float t);
	/// <summary>
	/// バック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Back(float t);
	/// <summary>
	/// ソフトバック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float SoftBack(float t);
	/// <summary>
	/// エラスティック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Elastic(float t);
	/// <summary>
	/// バウンド
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Bounce(float t);
	/// <summary>
	/// リニア
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static float Linear(float t);

private:
	static std::array<std::function<float(float)>, 12> funcPtr_;
};

class EaseData
{
public:
	EaseData(int count = 0);
	~EaseData() = default;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

public:
	/// <summary>
	/// 終了したか
	/// </summary>
	/// <returns></returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// 進行度の取得
	/// </summary>
	/// <returns>0.0 ~ 1.0</returns>
	float GetTimeRate() { return timeRate_; }
	/// <summary>
	/// フレームの設定
	/// </summary>
	/// <param name="count"></param>
	void SetCount(int count) { countNum_ = count; }
	/// <summary>
	/// 終了フラグの設定
	/// </summary>
	/// <param name="Flag"></param>
	void SetEndFlag(bool flag) { endFlag_ = flag; }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void SetActFlag(bool flag) { actFlag_ = flag; }
private:
	// イージングの進行度用
	float timer_ = 0.0f;
	float timeRate_ = 0.0f;
	int countNum_ = 0;
	bool endFlag_ = false;
	bool actFlag_ = true;
};