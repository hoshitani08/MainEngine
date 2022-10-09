#include "SpriteFactory.h"

void SpriteFactory::Initialize()
{
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(0, L"Resources/debugfont.png"))
	{
		assert(0);
	}
	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(0);

	// �e�N�X�`���ǂݍ���

	//UI : Player�֌W
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

	//UI : GamePlay�֌W
	if (!Sprite::LoadTexture(10, L"Resources/UI/play/clock.png"))
	{
		assert(0);
	}
}

SpriteFactory* SpriteFactory::GetInstance()
{
	static SpriteFactory instance;
	return &instance;
}