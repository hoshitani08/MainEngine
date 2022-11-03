#include "SpriteFactory.h"

void SpriteFactory::Initialize()
{
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(0, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// デバッグテキスト初期化
	DebugText::GetInstance()->Initialize(0);

	// テクスチャ読み込み
	

	//UI : GamePlay関係
	//playerのステータス
	if (!Sprite::LoadTexture(1, L"Resources/UI/play/frame.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(2, L"Resources/UI/play/lifegauge.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(3, L"Resources/UI/play/innerlifegauge.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(4, L"Resources/UI/play/strengthgauge.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(5, L"Resources/UI/play/attackUI.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(6, L"Resources/UI/play/defenseUI.png"))
	{
		assert(0);
	}

	// 時計
	if (!Sprite::LoadTexture(10, L"Resources/UI/play/clock.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(11, L"Resources/UI/play/clockNeedle.png"))
	{
		assert(0);
	}

	// ステージ
	if (!Sprite::LoadTexture(12, L"Resources/UI/play/map.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(13, L"Resources/UI/play/playerIcon.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(14, L"Resources/UI/play/enemyIcon.png"))
	{
		assert(0);
	}

	// アイテム関係
	if (!Sprite::LoadTexture(20, L"Resources/UI/play/enn.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(21, L"Resources/UI/item/potion.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(22, L"Resources/UI/item/attackBuff.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(23, L"Resources/UI/item/defenseBuff.png"))
	{
		assert(0);
	}

	// 数字
	if (!Sprite::LoadTexture(30, L"Resources/UI/number/0.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(31, L"Resources/UI/number/1.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(32, L"Resources/UI/number/2.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(33, L"Resources/UI/number/3.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(34, L"Resources/UI/number/4.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(35, L"Resources/UI/number/5.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(36, L"Resources/UI/number/6.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(37, L"Resources/UI/number/7.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(38, L"Resources/UI/number/8.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(39, L"Resources/UI/number/9.png"))
	{
		assert(0);
	}

	// ボタン
	if (!Sprite::LoadTexture(40, L"Resources/UI/play/LBbutton.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(41, L"Resources/UI/play/Bbutton.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(42, L"Resources/UI/play/Xbutton.png"))
	{
		assert(0);
	}

	if (!Sprite::LoadTexture(100, L"Resources/black.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(101, L"Resources/effect1.png"))
	{
		assert(0);
	}
	if (!Sprite::LoadTexture(102, L"Resources/UI/play/Loading.png"))
	{
		assert(0);
	}
}

SpriteFactory* SpriteFactory::GetInstance()
{
	static SpriteFactory instance;
	return &instance;
}