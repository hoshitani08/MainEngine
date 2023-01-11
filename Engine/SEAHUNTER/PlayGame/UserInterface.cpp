#include "UserInterface.h"
#include "DebugText.h"
#include "Input.h"
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
	// アンカーポイント
	XMFLOAT2 anchorPoint = { 0.5f, 0.5f };
	// 色
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	//playerのステータス
	lifeFrame_ =          Sprite::Create(1, { 47, 14 });
	strengthFrame_ =      Sprite::Create(1, { 47, 50 });
	strengthFrame_->SetIsFlipY(true);
	lifeGauge_ =          Sprite::Create(2, { 62, 16 });
	innerLifeGauge_ =     Sprite::Create(3, { 62, 16 });
	strengthGauge_ =      Sprite::Create(4, { 62, 37 });
	attackIcon_ =         Sprite::Create(5, { 70, 55 });
	defenseIcon_ =        Sprite::Create(6, { 90, 60 });
	frameBackground_[0] = Sprite::Create(7, { 47, 14 });
	frameBackground_[1] = Sprite::Create(7, { 47, 50 });
	frameBackground_[1]->SetIsFlipY(true);

	//enemyのステータス
	enemyLifeFrame_ = Sprite::Create(105, { 376, 620 });
	enemyLifeGauge_ =      Sprite::Create(3, { 392, 622 });
	enemyInnerLifeGauge_ = Sprite::Create(4, { 392, 622 });
	enemyText_ = Sprite::Create(104, { 640, 590 }, color, anchorPoint);
	frameBackground_[2] = Sprite::Create(106, { 376, 620 });

	// 時計
	clockFrame_ =  Sprite::Create(10, { 32, 32 }, color, anchorPoint);
	clockNeedle_ = Sprite::Create(11, { 32, 32 }, color, anchorPoint);

	// ステージ
	mapSprite_ =  Sprite::Create(12, { 50, 420 });
	mapSprite_->SetSize({ 200,200 });
	playerIcon_ = Sprite::Create(13, { 50, 420 }, color, anchorPoint);
	enemyIcon_ =  Sprite::Create(14, { 50, 420 }, color, anchorPoint);

	// アイテム

	XMFLOAT2 itemPos = { 1080, 580 };
	float itemSize = 0.7f;
	itemBackground_ = Sprite::Create(20, itemPos, color, anchorPoint);
	itemBackground_->SetScale(itemSize);
	for (int i = 0; i < itemSprite_.size(); i++)
	{
		itemSprite_[i] = Sprite::Create(21 + i, itemPos, color, anchorPoint);
		itemSprite_[i]->SetScale(itemSize);
	}
	itemFrame_ = Sprite::Create(24, itemPos, color, anchorPoint);
	itemFrame_->SetScale(itemSize);

	itemSBackground_ = Sprite::Create(25, itemPos, color, anchorPoint);
	itemSBackground_->SetScale(itemSize);
	itemSBackground_->SetSize({ 64, 126 });
	//最大 { 510, 125 }

	// 数字
	XMFLOAT2 countPos = { 1112, 625 };
	XMFLOAT2 size = { 16, 16 };
	for (int i = 0; i < oneDigits_.size(); i++)
	{
		oneDigits_[i] = Sprite::Create(30 + i, countPos, color, anchorPoint);
		oneDigits_[i]->SetSize(size);
	}
	for (int i = 0; i < tenDigits_.size(); i++)
	{
		tenDigits_[i] = Sprite::Create(30 + i, { countPos.x - 10, countPos.y }, color, anchorPoint);
		tenDigits_[i]->SetSize(size);
	}

	// ボタン
	lbButtonIcon_ = Sprite::Create(40, { itemPos.x - 60, itemPos.y }, color, anchorPoint);
	lbButtonIcon_->SetScale(itemSize);

	xButtonIcon_ = Sprite::Create(42, { itemPos.x - 30, countPos.y }, color, anchorPoint);
	xButtonIcon_->SetSize({ 32,32 });

	bButtonIcon_ = Sprite::Create(41, { itemPos.x + 30, countPos.y }, color, anchorPoint);
	bButtonIcon_->SetSize({ 32,32 });

	monsterHp_ = monster_->MAX_HP;
	hunterHp_ = hunter_->MAX_HP;
	hunterstamina_ = hunter_->MAX_STAMINA;

	// イージングの初期化
	monsterEase_ = std::make_unique<EaseData>(20);
	monsterEase_->SetActFlag(false);
	hunterEase_ = std::make_unique<EaseData>(20);
	hunterEase_->SetActFlag(false);
	frameEase_ = std::make_unique<EaseData>(5);
	frameEase_->SetActFlag(false);
	frameEase2_ = std::make_unique<EaseData>(5);
	frameEase2_->SetActFlag(false);
	selectionEase_ = std::make_unique<EaseData>(10);
	selectionEase_->SetActFlag(false);
	buttonEase_ = std::make_unique<EaseData>(10);
	buttonEase_->SetActFlag(false);
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
}

void UserInterface::BackDraw()
{
}

void UserInterface::NearDraw()
{
	for (auto& a : frameBackground_)
	{
		a->Draw();
	}
	innerLifeGauge_->Draw();
	strengthGauge_->Draw();
	lifeGauge_->Draw();
	strengthFrame_->Draw();
	lifeFrame_->Draw();

	clockFrame_->Draw();
	clockNeedle_->Draw();

	enemyInnerLifeGauge_->Draw();
	enemyLifeGauge_->Draw();
	enemyLifeFrame_->Draw();
	enemyText_->Draw();

	// アイテム関係
	itemBackground_->Draw();
	if (hunter_->GetItemSelectionFlag())
	{
		itemSBackground_->Draw();
		bButtonIcon_->Draw();
		for (auto& m : itemSprite_)
		{
			m->Draw();
		}
	}
	else
	{
		itemSprite_[hunter_->GetItemType()]->Draw();
	}
	itemFrame_->Draw();
	oneDigits_[oneCount_]->Draw();
	if (isTenCountFlag_)
	{
		tenDigits_[tenCount_]->Draw();
	}
	if (!hunter_->GetItemSelectionFlag())
	{
		lbButtonIcon_->Draw();
	}
	xButtonIcon_->Draw();

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
	if (lifeGauge_->GetSize().x < innerLifeGauge_->GetSize().x)
	{
		hunterEase_->SetActFlag(true);
		hunterEase_->Update();

		innerLifeGauge_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, innerLifeGauge_->GetSize(), lifeGauge_->GetSize(), hunterEase_->GetTimeRate()));

		if (hunterEase_->GetEndFlag())
		{
			hunterEase_->SetEndFlag(false);
			hunterEase_->Reset();
		}
	}

	if (enemyLifeGauge_->GetSize().x < enemyInnerLifeGauge_->GetSize().x)
	{
		monsterEase_->SetActFlag(true);
		monsterEase_->Update();

		enemyInnerLifeGauge_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, enemyInnerLifeGauge_->GetSize(), enemyLifeGauge_->GetSize(), monsterEase_->GetTimeRate()));

		if (monsterEase_->GetEndFlag())
		{
			monsterEase_->SetEndFlag(false);
			monsterEase_->Reset();
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

	if (ItemManager::GetInstance()->GetItemQuantity(hunter_->GetItemType()) >= ItemManager::GetInstance()->GetItemMaxCount(hunter_->GetItemType()))
	{
		XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

		for (int i = 0; i < oneDigits_.size(); i++)
		{
			oneDigits_[i]->SetColor(color);
		}
		for (int i = 0; i < tenDigits_.size(); i++)
		{
			tenDigits_[i]->SetColor(color);
		}
	}
	else
	{
		XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		for (int i = 0; i < oneDigits_.size(); i++)
		{
			oneDigits_[i]->SetColor(color);
		}
		for (int i = 0; i < tenDigits_.size(); i++)
		{
			tenDigits_[i]->SetColor(color);
		}
	}

	tenCount_ = count / 10;
	oneCount_ = (count - (tenCount_ * 10));

	XMFLOAT2 countPos = { 1112, 625 };
	if (count / 10 == 0)
	{
		oneDigits_[oneCount_]->SetPosition({ countPos.x - 5, countPos.y });
		isTenCountFlag_ = false;
	}
	else
	{
		oneDigits_[oneCount_]->SetPosition(countPos);
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

	// ボタンの位置
	XMFLOAT2 buttonPos = { 1080, 625 };
	// アイテム入れ替え時の背景のサイズ
	XMFLOAT2 size = { 510, 126 };
	// アイテムの中心の位置
	XMFLOAT2 itemPos = { 1080, 580 };
	// 最初の色
	XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	// 最後の色
	XMFLOAT4 endColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (hunter_->GetItemSelectionFlag())
	{
		frameEase2_->Reset();
		buttonReset_ = false;

		float itemAddPos = 100.0f;

		if (xButtonIcon_->GetPosition().x > buttonPos.x - 100)
		{
			frameEase_->SetActFlag(true);
			frameEase_->Update();

			xButtonIcon_->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, xButtonIcon_->GetPosition(), { buttonPos.x - 100, buttonPos.y }, frameEase_->GetTimeRate()));
			bButtonIcon_->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, bButtonIcon_->GetPosition(), { buttonPos.x + 100, buttonPos.y }, frameEase_->GetTimeRate()));
			itemSBackground_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemSBackground_->GetSize().x, size.y }, size, frameEase_->GetTimeRate()));

			bool countFlag = false;
			for (int i = 0; i < itemSprite_.size(); i++)
			{
				if (hunter_->GetItemType() != i)
				{
					itemSprite_[i]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, frameEase_->GetTimeRate()));
				}
			}
			if (hunter_->GetItemType() == (int)ItemManager::ItemType::DefenseBuff)
			{
				itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(itemPos);
				itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
				itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
			}
			else if (hunter_->GetItemType() == (int)ItemManager::ItemType::AttackBuff)
			{
				itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
				itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(itemPos);
				itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
			}
			else
			{
				itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
				itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, frameEase_->GetTimeRate()));
				itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(itemPos);
			}

			if (frameEase_->GetEndFlag())
			{
				hunter_->SetIsItemSelection(true);
				frameEase_->SetActFlag(false);
			}
		}
		else if(!hunter_->GetIsItemSelection())
		{
			selectionEase_->SetActFlag(true);
			selectionEase_->Update();

			if (hunter_->GetButtonFlag())
			{
				if (hunter_->GetItemType() == (int)ItemManager::ItemType::DefenseBuff)
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x - itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition({ itemPos.x - itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
				}
				else if (hunter_->GetItemType() == (int)ItemManager::ItemType::AttackBuff)
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition({ itemPos.x - itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x - itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
				}
				else
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x + itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition({ itemPos.x - itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x - itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
				}

				xButtonIcon_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, { 48, 48 }, { 32, 32 }, selectionEase_->GetTimeRate()));
			}
			else
			{
				if (hunter_->GetItemType() == (int)ItemManager::ItemType::DefenseBuff)
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x + itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition({ itemPos.x + itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
				}
				else if (hunter_->GetItemType() == (int)ItemManager::ItemType::AttackBuff)
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x + itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition({ itemPos.x + itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
				}
				else
				{
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetPosition({ itemPos.x + itemAddPos, itemPos.y });
					itemSprite_[(int)ItemManager::ItemType::DefenseBuff]->SetColor(Ease::Action(EaseType::In, EaseFunctionType::Quad, startColor, endColor, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::AttackBuff]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, itemPos, { itemPos.x - itemAddPos, itemPos.y }, selectionEase_->GetTimeRate()));
					itemSprite_[(int)ItemManager::ItemType::Healing]->SetPosition(Ease::Action(EaseType::In, EaseFunctionType::Quad, { itemPos.x + itemAddPos, itemPos.y }, itemPos, selectionEase_->GetTimeRate()));
				}

				bButtonIcon_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, { 48, 48 }, { 32, 32 }, selectionEase_->GetTimeRate()));
			}

			if (selectionEase_->GetEndFlag())
			{
				hunter_->SetIsItemSelection(true);
				selectionEase_->Reset();
			}
		}
	}
	else
	{
		frameEase_->Reset();

		if (!buttonReset_)
		{
			itemSprite_[hunter_->GetItemType()]->SetPosition(itemPos);
			itemSprite_[hunter_->GetItemType()]->SetColor(endColor);
			xButtonIcon_->SetSize({ 32, 32 });
			bButtonIcon_->SetSize({ 32, 32 });
			buttonReset_ = true;
		}
		

		if (xButtonIcon_->GetPosition().x < buttonPos.x - 30)
		{
			frameEase2_->SetActFlag(true);
			frameEase2_->Update();

			xButtonIcon_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, xButtonIcon_->GetPosition(), { buttonPos.x - 30, buttonPos.y }, frameEase2_->GetTimeRate()));
			bButtonIcon_->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quad, bButtonIcon_->GetPosition(), { buttonPos.x + 30, buttonPos.y }, frameEase2_->GetTimeRate()));
			itemSBackground_->SetSize(Ease::Action(EaseType::Out, EaseFunctionType::Quad, { itemSBackground_->GetSize().x, size.y }, { 64, size.y }, frameEase2_->GetTimeRate()));

			if (frameEase2_->GetEndFlag())
			{
				frameEase2_->SetActFlag(false);
			}
		}

		if (hunter_->GetButtonEaseFlag())
		{
			buttonEase_->SetActFlag(true);
			buttonEase_->Update();

			xButtonIcon_->SetSize(Ease::Action(EaseType::In, EaseFunctionType::Quad, { 48, 48 }, { 32, 32 }, buttonEase_->GetTimeRate()));

			if (buttonEase_->GetEndFlag())
			{
				hunter_->SetButtonEaseFlag(false);
				buttonEase_->SetActFlag(false);
				buttonEase_->Reset();
			}
		}
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
