#include "BaseCalculate.h"

#include <random>

using namespace DirectX;

const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const float rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x + rhs;
	result.y = lhs.y + rhs;
	return result;
}

const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const float rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	return result;
}

const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const float rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	return result;
}

const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const float rhs)
{
	XMFLOAT2 result = {};
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x + rhs;
	result.y = lhs.y + rhs;
	result.z = lhs.z + rhs;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	result.z = lhs.z - rhs;
	return result;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	result.z = lhs.z * rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	result.z = lhs.z / rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result = {};
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const float rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x + rhs;
	result.y = lhs.y + rhs;
	result.z = lhs.z + rhs;
	result.w = lhs.w + rhs;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const float rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	result.z = lhs.z - rhs;
	result.w = lhs.w - rhs;
	return result;
}

const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	result.z = lhs.z * rhs.z;
	result.w = lhs.w * rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lhs, const float rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	result.w = lhs.w * rhs;
	return result;
}

const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	result.z = lhs.z / rhs.z;
	result.w = lhs.w / rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const float rhs)
{
	XMFLOAT4 result = {};
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	result.w = lhs.w / rhs;
	return result;
}

const float RandCalculate(float a, float b)
{
	// �����Z���k�E�c�C�X�^�[�@�ɂ��[��������������A
	// �n�[�h�E�F�A�������V�[�h�ɂ��ď�����
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	// ��l�������z
	// [a, b)�̒l�͈̔͂ŁA���m���Ɏ����𐶐�����
	std::uniform_real_distribution<> dist1(a, b);

	return float(dist1(engine));
}
