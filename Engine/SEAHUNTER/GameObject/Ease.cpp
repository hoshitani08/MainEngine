#include "Ease.h"
#include "BaseCalculate.h"

#include<math.h>

using namespace DirectX;

const float Ease::PI = 3.14159265359f;
std::array<std::function<double(double)>, 12> Ease::funcPtr_ = { Quad, Cubic, Quart, Quint, Sine, Expo, Circ, Back, SoftBack, Elastic, Bounce, Linear };

const float Ease::easeIn(const float& start, const float& end, const float t)
{
	return start * (1.0f - t * t) + end * (t * t);
}

const XMFLOAT2 Ease::easeIn(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	float x, y, z = 0;

	x = start.x * (1.0f - t * t) + end.x * (t * t);
	y = start.y * (1.0f - t * t) + end.y * (t * t);

	return XMFLOAT2({ x, y });
}

const XMFLOAT3 Ease::easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	float x, y, z = 0;

	x = start.x * (1.0f - t * t) + end.x * (t * t);
	y = start.y * (1.0f - t * t) + end.y * (t * t);
	z = start.z * (1.0f - t * t) + end.z * (t * t);

	return XMFLOAT3({ x, y, z });
}

const XMFLOAT4 Ease::easeIn(const XMFLOAT4& start, const XMFLOAT4& end, const float t)
{
	float x, y, z, w = 0;

	x = start.x * (1.0f - t * t) + end.x * (t * t);
	y = start.y * (1.0f - t * t) + end.y * (t * t);
	z = start.z * (1.0f - t * t) + end.z * (t * t);
	w = start.w * (1.0f - t * t) + end.w * (t * t);

	return XMFLOAT4({x, y, z, w});
}

const float Ease::easeOut(const float& start, const float& end, const float t)
{
	return start * (1.0f - (t * (2 - t))) + end * (t * (2 - t));
}

const XMFLOAT2 Ease::easeOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	float x, y = 0;

	x = start.x * (1.0f - (t * (2 - t))) + end.x * (t * (2 - t));
	y = start.y * (1.0f - (t * (2 - t))) + end.y * (t * (2 - t));

	return XMFLOAT2({ x, y });
}

const XMFLOAT3 Ease::easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	float x, y, z = 0;

	x = start.x * (1.0f - (t * (2 - t))) + end.x * (t * (2 - t));
	y = start.y * (1.0f - (t * (2 - t))) + end.y * (t * (2 - t));
	z = start.z * (1.0f - (t * (2 - t))) + end.z * (t * (2 - t));

	return XMFLOAT3({ x, y, z});
}

const XMFLOAT4 Ease::easeOut(const XMFLOAT4& start, const XMFLOAT4& end, const float t)
{
	float x, y, z, w = 0;

	x = start.x * (1.0f - (t * (2 - t))) + end.x * (t * (2 - t));
	y = start.y * (1.0f - (t * (2 - t))) + end.y * (t * (2 - t));
	z = start.z * (1.0f - (t * (2 - t))) + end.z * (t * (2 - t));
	w = start.w * (1.0f - (t * (2 - t))) + end.w * (t * (2 - t));

	return XMFLOAT4({ x, y, z, w});
}

const float Ease::easeInOut(const float& start, const float& end, const float t)
{
	return start * (1.0f - (t * t) * (3 - (2 * t))) + end * (t * t) * (3 - (2 * t));
}

const XMFLOAT2 Ease::easeInOut(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	float x, y = 0;

	x = start.x * (1.0f - (t * t) * (3 - (2 * t))) + end.x * (t * t) * (3 - (2 * t));
	y = start.y * (1.0f - (t * t) * (3 - (2 * t))) + end.y * (t * t) * (3 - (2 * t));

	return XMFLOAT2({ x, y });
}

const XMFLOAT3 Ease::easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	float x, y, z = 0;

	x = start.x * (1.0f - (t * t) * (3 - (2 * t))) + end.x * (t * t) * (3 - (2 * t));
	y = start.y * (1.0f - (t * t) * (3 - (2 * t))) + end.y * (t * t) * (3 - (2 * t));
	z = start.z * (1.0f - (t * t) * (3 - (2 * t))) + end.z * (t * t) * (3 - (2 * t));

	return XMFLOAT3({ x, y, z });
}

const XMFLOAT4 Ease::easeInOut(const XMFLOAT4& start, const XMFLOAT4& end, const float t)
{
	float x, y, z, w = 0;

	x = start.x * (1.0f - (t * t) * (3 - (2 * t))) + end.x * (t * t) * (3 - (2 * t));
	y = start.y * (1.0f - (t * t) * (3 - (2 * t))) + end.y * (t * t) * (3 - (2 * t));
	z = start.z * (1.0f - (t * t) * (3 - (2 * t))) + end.z * (t * t) * (3 - (2 * t));
	w = start.w * (1.0f - (t * t) * (3 - (2 * t))) + end.w * (t * t) * (3 - (2 * t));

	return XMFLOAT4({ x, y, z, w });
}

const float Ease::lerp(const float& start, const float& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

const XMFLOAT2 Ease::lerp(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
	float x, y = 0;

	x = start.x * (1.0f - t) + end.x * t;
	y = start.y * (1.0f - t) + end.y * t;

	return XMFLOAT2({ x, y });
}

const XMFLOAT3 Ease::lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	float x, y, z = 0;

	x = start.x * (1.0f - t) + end.x * t;
	y = start.y * (1.0f - t) + end.y * t;
	z = start.z * (1.0f - t) + end.z * t;

	return XMFLOAT3({ x, y, z });
}

const XMFLOAT4 Ease::lerp(const XMFLOAT4& start, const XMFLOAT4& end, const float t)
{
	float x, y, z, w = 0;

	x = start.x * (1.0f - t) + end.x * t;
	y = start.y * (1.0f - t) + end.y * t;
	z = start.z * (1.0f - t) + end.z * t;
	w = start.w * (1.0f - t) + end.w * t;

	return XMFLOAT4({ x, y, z, w });
}

double Ease::Quad(double t)
{
	return t * t;
}

double Ease::Cubic(double t)
{
	return t * t * t;
}

double Ease::Quart(double t)
{
	return t * t * t * t;
}

double Ease::Quint(double t)
{
	return t * t * t * t * t;
}

double Ease::Sine(double t)
{
	return 1 - cos(t * PI / 2);
}

double Ease::Expo(double t)
{
	return pow(2, -(1 - t) * 10);
}

double Ease::Circ(double t)
{
	return 1 - sqrt((0 > 1 - t * t) ? 0 : 1 - t * t);
}

double Ease::Back(double t)
{
	double c1 = 1.70158;
	double c3 = c1 + 1;

	return t * t * (c3 * t - c1);
}

double Ease::SoftBack(double t)
{
	return t * t * (2 * t - 1);
}

double Ease::Elastic(double t)
{
	return 56 * t * t * t * t * t - 105 * t * t * t * t + 60 * t * t * t - 10 * t * t;
}

double Ease::Bounce(double t)
{
	double pow2;
	int bounce = 4;
	while (t < ((pow2 = pow(2, --bounce)) - 1) / 11) {}
	return 1 / pow(4, 3 - bounce) - 7.5625 * pow((pow2 * 3 - 2) / 22 - t, 2);
}

double Ease::Linear(double t)
{
	return t;
}
