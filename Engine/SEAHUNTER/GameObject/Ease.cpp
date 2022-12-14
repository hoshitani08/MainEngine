#include "Ease.h"
#include "BaseCalculate.h"

#include<math.h>

using namespace DirectX;

const float Ease::PI = 3.14159265359f;
std::array<std::function<float(float)>, 12> Ease::funcPtr_ = { Quad, Cubic, Quart, Quint, Sine, Expo, Circ, Back, SoftBack, Elastic, Bounce, Linear };

const float Ease::Action(EaseType easeType, EaseFunctionType easeFunctionType, const float& start, const float& end, const float t)
{
	if (easeType == EaseType::In)
	{
		return (end - start) * funcPtr_[(int)easeFunctionType](t) + start;
	}
	else if (easeType == EaseType::Out)
	{
		return (end - start) * (1.0f - funcPtr_[(int)easeFunctionType](1.0f - t)) + start;
	}
	else if (easeType == EaseType::InOut)
	{
		if (t < 0.5f)
		{
			return (end - start) * (funcPtr_[(int)easeFunctionType](t * 2.0f) / 2.0f) + start;
		}

		return (end - start) * (1.0f - funcPtr_[(int)easeFunctionType](2.0f - 2.0f * t) / 2.0f) + start;
	}

	return 0.0f;
}

const XMFLOAT2 Ease::Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	XMFLOAT2 result = {};
	result.x = Action(easeType, easeFunctionType, start.x, end.x, t);
	result.y = Action(easeType, easeFunctionType, start.y, end.y, t);
	return result;
}

const XMFLOAT3 Ease::Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 result = {};
	result.x = Action(easeType, easeFunctionType, start.x, end.x, t);
	result.y = Action(easeType, easeFunctionType, start.y, end.y, t);
	result.z = Action(easeType, easeFunctionType, start.z, end.z, t);
	return result;
}

const XMFLOAT4 Ease::Action(EaseType easeType, EaseFunctionType easeFunctionType, const XMFLOAT4& start, const XMFLOAT4& end, const float t)
{
	XMFLOAT4 result = {};
	result.x = Action(easeType, easeFunctionType, start.x, end.x, t);
	result.y = Action(easeType, easeFunctionType, start.y, end.y, t);
	result.z = Action(easeType, easeFunctionType, start.z, end.z, t);
	result.w = Action(easeType, easeFunctionType, start.w, end.w, t);
	return result;
}

float Ease::Quad(float t)
{
	return t * t;
}

float Ease::Cubic(float t)
{
	return t * t * t;
}

float Ease::Quart(float t)
{
	return t * t * t * t;
}

float Ease::Quint(float t)
{
	return t * t * t * t * t;
}

float Ease::Sine(float t)
{
	return 1 - cosf(t * PI / 2.0f);
}

float Ease::Expo(float t)
{
	return pow(2.0f, -(1.0f - t) * 10.0f);
}

float Ease::Circ(float t)
{
	return 1.0f - sqrtf((0.0f > 1.0f - t * t) ? 0.0f : 1.0f - t * t);
}

float Ease::Back(float t)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1.0f;

	return t * t * (c3 * t - c1);
}

float Ease::SoftBack(float t)
{
	return t * t * (2.0f * t - 1.0f);
}

float Ease::Elastic(float t)
{
	return 56.0f * t * t * t * t * t - 105.0f * t * t * t * t + 60.0f * t * t * t - 10.0f * t * t;
}

float Ease::Bounce(float t)
{
	float pow2 = 0.0f;
	float bounce = 4.0f;
	while (t < ((pow2 = powf(2.0f, --bounce)) - 1.0f) / 11.0f) {}
	return 1 / powf(4.0f, 3.0f - bounce) - 7.5625f * powf((pow2 * 3.0f - 2.0f) / 22.0f - t, 2.0f);
}

float Ease::Linear(float t)
{
	return t;
}

EaseData::EaseData(int count)
{
	countNum_ = count;
}

void EaseData::Update()
{
	if (!endFlag_ && actFlag_)
	{
		timeRate_ = timer_ / countNum_;
		timer_++;

		if (timer_ > countNum_)
		{
			endFlag_ = true;
			timer_ = 0.0f;
		}
	}
}

void EaseData::Reset()
{
	timer_ = 0.0f;
	timeRate_ = 0.0f;
	endFlag_ = false;
}
