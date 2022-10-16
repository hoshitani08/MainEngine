#include "UserInterface.h"

#include "DebugText.h"
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

	enemyLifeFrame_ = Sprite::Create(1, { 390, 620 });
	enemyLifeGauge_ = Sprite::Create(3, { 392, 622 });
	enemyInnerLifeGauge_ = Sprite::Create(4, { 392, 622 });

	monsterHp_ = monster_->MAX_HP;
	hunterHp_ = hunter_->MAX_HP;
	hunterstamina_ = hunter_->MAX_STAMINA;
}

void UserInterface::Finalize()
{
}

void UserInterface::Update()
{
	DamageCalculate();
	StrengthCalculate();

	HpEase();

	if (!isPlayerDeath_ && lifeGauge_->GetSize().x < 1.0f)
	{
		isPlayerDeath_ = true;
	}

	DebugText::GetInstance()->Print("ENEMY", 615, 580, 1.5f);
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

	enemyLifeFrame_->Draw();
	enemyInnerLifeGauge_->Draw();
	enemyLifeGauge_->Draw();
}

void UserInterface::HpEase()
{
	float countNum = 20;

	if (lifeGauge_->GetSize().x < innerLifeGauge_->GetSize().x)
	{
		float timeRate = hunterEaseTimer_ / countNum;
		hunterEaseTimer_++;

		innerLifeGauge_->SetSize(Ease::easeIn(innerLifeGauge_->GetSize(), lifeGauge_->GetSize(), timeRate));

		if (hunterEaseTimer_ > countNum)
		{
			hunterEaseTimer_ = 0;
		}
	}

	if (enemyLifeGauge_->GetSize().x < enemyInnerLifeGauge_->GetSize().x)
	{
		float timeRate = monsterEaseTimer_ / countNum;
		monsterEaseTimer_++;

		enemyInnerLifeGauge_->SetSize(Ease::easeIn(enemyInnerLifeGauge_->GetSize(), enemyLifeGauge_->GetSize(), timeRate));

		if (monsterEaseTimer_ > countNum)
		{
			monsterEaseTimer_ = 0;
		}
	}
}

void UserInterface::DamageCalculate()
{
	//全体の何パーセントかという計算は ある数 ÷ 全体 × 100
	//全体の何パーセントはいくつかという計算は、 全体 × パーセント ÷ 100
	if (hunterHp_ != hunter_->GetHp())
	{
		XMFLOAT2 hpGaugeSize = lifeGauge_->GetSize();

		float saveCount = hunter_->GetHp() / hunter_->MAX_HP * 100;

		hpGaugeSize.x = (508 * saveCount) / 100;

		lifeGauge_->SetSize(hpGaugeSize);
		hunterHp_ = hunter_->GetHp();
	}

	if (lifeGauge_->GetSize().x > innerLifeGauge_->GetSize().x)
	{
		innerLifeGauge_->SetSize(lifeGauge_->GetSize());
	}

	//全体の何パーセントかという計算は ある数 ÷ 全体 × 100
	//全体の何パーセントはいくつかという計算は、 全体 × パーセント ÷ 100
	if (monsterHp_ > monster_->GetHp())
	{
		XMFLOAT2 hpGaugeSize = enemyLifeGauge_->GetSize();

		float saveCount = monster_->GetHp() / monster_->MAX_HP * 100;

		hpGaugeSize.x = (508 * saveCount) / 100;

		enemyLifeGauge_->SetSize(hpGaugeSize);
		monsterHp_ = monster_->GetHp();
	}
}

void UserInterface::StrengthCalculate()
{
	if (hunterstamina_ != hunter_->GetStamina())
	{
		XMFLOAT2 strengthGaugeSize = strengthGauge_->GetSize();

		float saveCount = hunter_->GetStamina() / hunter_->MAX_STAMINA * 100;

		strengthGaugeSize.x = (508 * saveCount) / 100;

		strengthGauge_->SetSize(strengthGaugeSize);
		hunterstamina_ = hunter_->GetStamina();
	}
	else if (!hunter_->GetAvoidFlag())
	{
		float stamina = hunter_->GetStamina();

		stamina++;

		if (stamina >= hunter_->MAX_STAMINA)
		{
			stamina = hunter_->MAX_STAMINA;
		}
		hunter_->SetStamina(stamina);
	}
}