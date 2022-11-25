#pragma once
#include <string>
#include <cstdlib>
#include <vector>
#include <complex>

#include <DirectXMath.h>

class Vector3 : public DirectX::XMFLOAT3
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector3();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">x成分</param>
	/// <param name="y">y成分</param>
	/// <param name="z">z成分</param>
	Vector3(float x, float y, float z);
	/// <summary>
	/// ノルム(長さ)を求める
	/// </summary>
	/// <returns></returns>
	float Length() const;
	/// <summary>
	/// 正規化する
	/// </summary>
	/// <returns></returns>
	Vector3& Normalize();
	/// <summary>
	/// 内積を求める
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Dot(const Vector3& v) const;
	/// <summary>
	/// 外積を求める
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& v) const;

	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);