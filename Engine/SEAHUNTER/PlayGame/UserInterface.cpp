#include "UserInterface.h"
#include "DebugText.h"
#include "Input.h"
#include "Ease.h"
#include "ItemManager.h"

UserInterface::UserInterface(Hunter* hunter, Monster* monster)
{
	hunter_ = hunter;
	monster_ = monster;
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	//playerのステータス
	lifeFrame_ =      Sprite::Create(1, { 60, 17 });
	strengthFrame_ =  Sprite::Create(1, { 60, 35 });
	lifeGauge_ =      Sprite::Create(2, { 62, 19 });
	innerLifeGauge_ = Sprite::Create(3, { 62, 19 });
	strengthGauge_ =  Sprite::Create(4, { 62, 37 });
	attackIcon_ =     Sprite::Create(5, { 70, 55 });
	defenseIcon_ =    Sprite::Create(6, { 90, 60 });

	//enemyのステータス
	enemyLifeFrame_ =      Sprite::Create(1, { 390, 620 });
	enemyLifeGauge_ =      Sprite::Create(3, { 392, 622 });
	enemyInnerLifeGauge_ = Sprite::Create(4, { 392, 622 });

	// 時計
	clockFrame_ =  Sprite::Create(10, { 0,0 });
	clockNeedle_ = Sprite::Create(11, { 32, 32 }, { 1,1,1,1 }, { 0.5f,0.5f });

	// ステージ
	mapSprite_ =  Sprite::Create(12, { 50, 420 });
	mapSprite_->SetSize({ 200,200 });
	playerIcon_ = Sprite::Create(13, { 50, 420 }, { 1,1,1,1 }, { 0.5f,0.5f });
	enemyIcon_ =  Sprite::Create(14, { 50, 420 }, { 1,1,1,1 }, { 0.5f,0.5f });

	// アイテム
	itemFrame_ = Sprite::Create(20, { 1020, 530 });
	for (int i = 0; i < itemSprite_.size(); i++)
	{
		itemSprite_[i] = Sprite::Create(21 + i, { 1050, 550 });
	}

	// 数字
	XMFLOAT2 size = { 16, 16 };
	for (int i = 0; i < oneDigits_.size(); i++)
	{
		oneDigits_[i] = Sprite::Create(30 + i, { 1110, 610 });
		oneDigits_[i]->SetSize(size);
	}
	for (int i = 0; i < tenDigits_.size(); i++)
	{
		tenDigits_[i] = Sprite::Create(30 + i, { 1100, 610 });
		tenDigits_[i]->SetSize(size);
	}

	// ボタン
	lbButtonIcon_ = Sprite::Create(40, { 970, 560 });

	xButtonIcon_ = Sprite::Create(42, { 1065, 640 });
	xButtonIcon_->SetSize({ 32,32 });

	bButtonIcon_ = Sprite::Create(41, { 1095, 640 });
	bButtonIcon_->SetSize({ 32,32 });

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
	ItemSelection();
	Map();

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
	clockNeedle_->Draw();

	enemyLifeFrame_->Draw();
	enemyInnerLifeGauge_->Draw();
	enemyLifeGauge_->Draw();

	// アイテム関係
	itemFrame_->Draw();
	itemSprite_[hunter_->GetItemType()]->Draw();
	oneDigits_[oneCount_]->Draw();
	if (isTenCountFlag_)
	{
		tenDigits_[tenCount_]->Draw();
	}
	lbButtonIcon_->Draw();
	xButtonIcon_->Draw();
	if (hunter_->GetItemSelectionFlag())
	{
		bButtonIcon_->Draw();
	}

	if (ItemManager::GetInstance()->IsAttackBuff())
	{
		if (ItemManager::GetInstance()->GetAttackBuffSecondTimer() < 100 ||
			ItemManager::GetInstance()->GetAttackBuffSecondTimer() >= 100 && ItemManager::GetInstance()->GetAttackBuffTimer() >= 30)
		{
			attackIcon_->Draw();
		}
	}
	if (ItemManager::GetInstance()->IsDefenseBuff())
	{
		if (ItemManager::GetInstance()->GetDefenseBuffSecondTimer() < 100 ||
			ItemManager::GetInstance()->GetDefenseBuffSecondTimer() >= 100 && ItemManager::GetInstance()->GetDefenseBuffTimer() >= 30)
		{
			defenseIcon_->Draw();
		}
	}

	//マップ関係
	mapSprite_->Draw();
	enemyIcon_->Draw();
	playerIcon_->Draw();
}

void UserInterface::HpEase()
{
	float countNum = 20;

	if (lifeGauge_->GetSize().x < innerLifeGauge_->GetSize().x)
	{
		float timeRate = hunterEaseTimer_ / countNum;
		hunterEaseTimer_++;

		innerLifeGauge_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, innerLifeGauge_->GetSize(), lifeGauge_->GetSize(), timeRate));

		if (hunterEaseTimer_ > countNum)
		{
			hunterEaseTimer_ = 0;
		}
	}

	if (enemyLifeGauge_->GetSize().x < enemyInnerLifeGauge_->GetSize().x)
	{
		float timeRate = monsterEaseTimer_ / countNum;
		monsterEaseTimer_++;

		enemyInnerLifeGauge_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, enemyInnerLifeGauge_->GetSize(), enemyLifeGauge_->GetSize(), timeRate));

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

		if (hpGaugeSize.x <= 0.0f)
		{
			hpGaugeSize.x = 0.0f;
		}

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

		if (hpGaugeSize.x <= 0.0f)
		{
			hpGaugeSize.x = 0.0f;
		}

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

void UserInterface::ItemSelection()
{
	int count = ItemManager::GetInstance()->GetItemQuantity(hunter_->GetItemType());

	tenCount_ = count / 10;
	oneCount_ = (count - (tenCount_ * 10));
	if (count / 10 == 0)
	{
		oneDigits_[oneCount_]->SetPosition({ 1105, 610 });
		isTenCountFlag_ = false;
	}
	else
	{
		oneDigits_[oneCount_]->SetPosition({ 1110, 610 });
		isTenCountFlag_ = true;
	}

	if (ItemManager::GetInstance()->IsAttackBuff())
	{
		defenseIcon_->SetPosition({ 90, 60 });
	}
	else
	{
		defenseIcon_->SetPosition({ 70, 60 });
	}

	if (hunter_->GetItemSelectionFlag())
	{
		xButtonIcon_->SetPosition({ 1040, 640 });
	}
	else
	{
		xButtonIcon_->SetPosition({ 1065, 640 });
	}
}

void UserInterface::Map()
{
	XMFLOAT2 b = { mapSprite_->GetSize().x / 2, mapSprite_->GetSize().y / 2 };

	playerIcon_->SetPosition({ (50 + b.x + hunter_->GetPosition().x), (420 + b.y + -hunter_->GetPosition().z) });
	playerIcon_->SetRotation(hunter_->GetRotation().y);

	enemyIcon_->SetPosition({ (50 + b.x + monster_->GetPosition().x), (420 + b.y + -monster_->GetPosition().z) });
	enemyIcon_->SetRotation(monster_->GetRotation().y);
}

void UserInterface::ClockCalculate(int timer)
{
	float count = (float)timer / 15.0f * 100;
	float count2 = (90 * count) / 100;
	clockNeedle_->SetRotation(count2);
}
