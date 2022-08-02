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
	lifeFrame_ =      Sprite::Create(1, { 55, 17 });
	strengthFrame_ =  Sprite::Create(1, { 55, 35 });
	lifeGauge_ =      Sprite::Create(2, { 57, 18 });
	innerLifeGauge_ = Sprite::Create(3, { 57, 18 });
	strengthGauge_ =  Sprite::Create(4, { 57, 37 });
	clockFrame_ = Sprite::Create(10, { 0,0 });
}

void UserInterface::Finalize()
{
}

void UserInterface::Update()
{
	Input* input = Input::GetInstance();
	if (lifeGauge_->GetSize().x < innerLifeGauge_->GetSize().x)
	{
		HpEase();
	}

	hpGaugeSize = lifeGauge_->GetSize();

	if (input->TriggerKey(DIK_1))
	{
		hpGaugeSize.x -= 30;
	}

	lifeGauge_->SetSize(hpGaugeSize);
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
	timeRate = easeTimer / countNum;
	easeTimer++;

	innerLifeGauge_->SetSize(Ease::easeIn(innerLifeGauge_->GetSize(), lifeGauge_->GetSize(), timeRate));

	if (easeTimer > countNum)
	{
		easeTimer = 0;
	}
}
