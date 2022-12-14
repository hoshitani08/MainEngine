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
	static const float Action(EaseType easeType, EaseFunctionType easeFunctionType, const float& start, const float& end, const float t);
	static const XMFLOAT2 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT2& start, const XMFLOAT2& end, const float t);
	static const XMFLOAT3 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	static const XMFLOAT4 Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT4& start, const XMFLOAT4& end, const float t);

private:
	/// <summary>
	/// �N���b�h
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Quad(float t);
	/// <summary>
	/// �L���[�r�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Cubic(float t);
	/// <summary>
	/// �N�H�[�g
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Quart(float t);
	/// <summary>
	/// �N�C���g
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Quint(float t);
	/// <summary>
	/// �T�C��
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Sine(float t);
	/// <summary>
	/// �G�L�X�|
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Expo(float t);
	/// <summary>
	/// �T�[�N��
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Circ(float t);
	/// <summary>
	/// �o�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Back(float t);
	/// <summary>
	/// �\�t�g�o�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float SoftBack(float t);
	/// <summary>
	/// �G���X�e�B�b�N
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Elastic(float t);
	/// <summary>
	/// �o�E���h
	/// </summary>
	/// <param name="t">����</param>
	/// <returns></returns>
	static float Bounce(float t);
	/// <summary>
	/// ���j�A
	/// </summary>
	/// <param name="t">����</param>
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
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

public:
	/// <summary>
	/// �I��������
	/// </summary>
	/// <returns></returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// �i�s�x�̎擾
	/// </summary>
	/// <returns>0.0 ~ 1.0</returns>
	float GetTimeRate() { return timeRate_; }
	/// <summary>
	/// �t���[���̐ݒ�
	/// </summary>
	/// <param name="count"></param>
	void SetCount(int count) { countNum_ = count; }
	/// <summary>
	/// �I���t���O�̐ݒ�
	/// </summary>
	/// <param name="Flag"></param>
	void SetEndFlag(bool flag) { endFlag_ = flag; }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag"></param>
	void SetActFlag(bool flag) { actFlag_ = flag; }
private:
	// �C�[�W���O�̐i�s�x�p
	float timer_ = 0.0f;
	float timeRate_ = 0.0f;
	int countNum_ = 0;
	bool endFlag_ = false;
	bool actFlag_ = true;
};