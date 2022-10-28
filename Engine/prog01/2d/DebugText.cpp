#include "DebugText.h"

DebugText::DebugText()
{
}

DebugText::~DebugText()
{

}

DebugText* DebugText::GetInstance()
{
	static DebugText instance;
	return &instance;
}

void DebugText::Initialize(UINT texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas_); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas_[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void DebugText::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		// �ő啶��������
		if (spriteIndex_ >= MAX_CHAR_COUNT)
		{
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / FONT_LINECOUNT;
		int fontIndexX = fontIndex % FONT_LINECOUNT;

		// ���W�v�Z
		spriteDatas_[spriteIndex_]->SetPosition({ x + FONT_WIDTH * scale * i, y });
		spriteDatas_[spriteIndex_]->SetTextureRect({ (float)fontIndexX * FONT_WIDTH, (float)fontIndexY * FONT_HEIGHT }, { (float)FONT_WIDTH, (float)FONT_HEIGHT });
		spriteDatas_[spriteIndex_]->SetSize({ FONT_WIDTH * scale, FONT_HEIGHT * scale });

		// �������P�i�߂�
		spriteIndex_++;
	}
}

void DebugText::VariablePrint(float x, float y, const std::string& text, float i, float size = 1.0f)
{
	variable_.m128_f32[0] = i;

	std::ostringstream spherestr;
	spherestr << text + " : "
		<< std::fixed << std::setprecision(2)
		<< variable_.m128_f32[0];

	Print(spherestr.str(), x, y, size);
}

// �܂Ƃ߂ĕ`��
void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex_; i++)
	{
		// �X�v���C�g�`��
		spriteDatas_[i]->Draw();
	}

	spriteIndex_ = 0;
}