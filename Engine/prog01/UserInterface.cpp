#include "UserInterface.h"

#include "Input.h"
#include "Ease.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	lifeFrame_ =      Sprite::Create(1, { 60, 17 });
	strengthFrame_ =  Sprite::Create(1, { 60, 35 });
	lifeGauge_ =      Sprite::Create(2, { 62, 19 });
	innerLifeGauge_ = Sprite::Create(3, { 62, 19 });
	strengthGauge_ =  Sprite::Create(4, { 62, 37 });
	clockFrame_ = Sprite::Create(10, { 0,0 });
}

void UserInterface::Finalize()
{
}

void UserInterface::Update()
{
	DamageCalculate();
	StrengthCalculate();

	if (lifeGauge_->GetSize().x < innerLifeGauge_->GetSize().x)
	{
		HpEase();
	}
}

void UserInterface::BackDraw()
{
}

void UserInterface::NearDraw()
{
	strengthFrame_->Draw();
	lifeFrame_->Draw();
	innerLifeGauge_->Draw();
	strengthGauge_->Draw();
	lifeGauge_->Draw();

	clockFrame_->Draw();
}

void UserInterface::HpEase()
{
	float timeRate = 0.0f;
	float countNum = 20;
	timeRate = easeTimer_ / countNum;
	easeTimer_++;

	innerLifeGauge_->SetSize(Ease::easeIn(innerLifeGauge_->GetSize(), lifeGauge_->GetSize(), timeRate));

	if (easeTimer_ > countNum)
	{
		easeTimer_ = 0;
	}
}

void UserInterface::DamageCalculate()
{
	XMFLOAT2 hpGaugeSize = lifeGauge_->GetSize();

	if (hunter_->GetDamageFlag())
	{
		hpGaugeSize.x -= (508 * hunter_->GetDamagePercent()) / 100;

		if (hpGaugeSize.x <= 0)
		{
			hpGaugeSize.x = 0.0f;
			isDeath_ = true;
		}

		hunter_->SetDamageFlag(false);
	}

	lifeGauge_->SetSize(hpGaugeSize);
}

void UserInterface::StrengthCalculate()
{
	XMFLOAT2 strengthGaugeSize = strengthGauge_->GetSize();

	if (hunter_->GetStrengthDecrement() != 0)
	{
		strengthGaugeSize.x -= (508 * hunter_->GetStrengthDecrement()) / 100;

		strengthGauge_->SetSize(strengthGaugeSize);
	}
	else if (!hunter_->GetAvoidFlag())
	{
		strengthGaugeSize.x += (508 * 1) / 100;

		if (strengthGaugeSize.x >= 508)
		{
			strengthGaugeSize.x = 508;
		}

		strengthGauge_->SetSize(strengthGaugeSize);
	}
}