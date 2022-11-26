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
	static const float easeIn(const float& start, const float& end, const float t);
	static const XMFLOAT2 easeIn(const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 easeIn(const XMFLOAT4& start, const XMFLOAT4& end, const float t);

	static const float easeOut(const float& start, const float& end, const float t);
	static const XMFLOAT2 easeOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 easeOut(const XMFLOAT4& start, const XMFLOAT4& end, const float t);

	static const float easeInOut(const float& start, const float& end, const float t);
	static const XMFLOAT2 easeInOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 easeInOut(const XMFLOAT4& start, const XMFLOAT4& end, const float t);

	static const float lerp(const float& start, const float& end, const float t);
	static const XMFLOAT2 lerp(const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 lerp(const XMFLOAT4& start, const XMFLOAT4& end, const float t);

private:
	/// <summary>
	/// クワッド
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Quad(double t);
	/// <summary>
	/// キュービック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Cubic(double t);
	/// <summary>
	/// クォート
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Quart(double t);
	/// <summary>
	/// クイント
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Quint(double t);
	/// <summary>
	/// サイン
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Sine(double t);
	/// <summary>
	/// エキスポ
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Expo(double t);
	/// <summary>
	/// サークル
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Circ(double t);
	/// <summary>
	/// バック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Back(double t);
	/// <summary>
	/// ソフトバック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double SoftBack(double t);
	/// <summary>
	/// エラスティック
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Elastic(double t);
	/// <summary>
	/// バウンド
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Bounce(double t);
	/// <summary>
	/// リニア
	/// </summary>
	/// <param name="t">時間</param>
	/// <returns></returns>
	static double Linear(double t);

private:
	static std::array<std::function<double(double)>, 12> funcPtr_;
};