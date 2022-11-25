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
	/// �R���X�g���N�^
	/// </summary>
	Vector3();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="x">x����</param>
	/// <param name="y">y����</param>
	/// <param name="z">z����</param>
	Vector3(float x, float y, float z);
	/// <summary>
	/// �m����(����)�����߂�
	/// </summary>
	/// <returns></returns>
	float Length() const;
	/// <summary>
	/// ���K������
	/// </summary>
	/// <returns></returns>
	Vector3& Normalize();
	/// <summary>
	/// ���ς����߂�
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Dot(const Vector3& v) const;
	/// <summary>
	/// �O�ς����߂�
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& v) const;

	// �P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	// ������Z�q�I�[�o�[���[�h
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