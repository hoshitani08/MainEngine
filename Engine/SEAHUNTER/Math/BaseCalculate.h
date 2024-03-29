#pragma once
#include <DirectXMath.h>

// XMFLOAT2
const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const float rhs);
const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const float rhs);
const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const float rhs);
const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const float rhs);

// XMFLOAT3
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float rhs);
const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const float rhs);
const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const float rhs);
const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float rhs);

// XMFLOAT4
const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const float rhs);
const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const float rhs);
const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lhs, const float rhs);
const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const float rhs);

/// <summary>
/// ランダムの計算
/// </summary>
/// <param name="a">最小値</param>
/// <param name="b">最大値</param>
const float RandCalculate(float a, float b);