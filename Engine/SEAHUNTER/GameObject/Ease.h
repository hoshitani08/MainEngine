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
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: // �萔
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
	/// �N���b�h
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Quad(double t);
	/// <summary>
	/// �L���[�r�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Cubic(double t);
	/// <summary>
	/// �N�H�[�g
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Quart(double t);
	/// <summary>
	/// �N�C���g
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Quint(double t);
	/// <summary>
	/// �T�C��
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Sine(double t);
	/// <summary>
	/// �G�L�X�|
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Expo(double t);
	/// <summary>
	/// �T�[�N��
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Circ(double t);
	/// <summary>
	/// �o�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Back(double t);
	/// <summary>
	/// �\�t�g�o�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double SoftBack(double t);
	/// <summary>
	/// �G���X�e�B�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Elastic(double t);
	/// <summary>
	/// �o�E���h
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Bounce(double t);
	/// <summary>
	/// ���j�A
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static double Linear(double t);

private:
	static std::array<std::function<double(double)>, 12> funcPtr_;
};