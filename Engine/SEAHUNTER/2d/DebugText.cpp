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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas_); i++)
	{
		// スプライトを生成する
		spriteDatas_[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void DebugText::Print(const std::string& text, float x, float y, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex_ >= MAX_CHAR_COUNT)
		{
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / FONT_LINECOUNT;
		int fontIndexX = fontIndex % FONT_LINECOUNT;

		// 座標計算
		spriteDatas_[spriteIndex_]->SetPosition({ x + FONT_WIDTH * scale * i, y });
		spriteDatas_[spriteIndex_]->SetTextureRect({ (float)fontIndexX * FONT_WIDTH, (float)fontIndexY * FONT_HEIGHT }, { (float)FONT_WIDTH, (float)FONT_HEIGHT });
		spriteDatas_[spriteIndex_]->SetSize({ FONT_WIDTH * scale, FONT_HEIGHT * scale });

		// 文字を１つ進める
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

// まとめて描画
void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex_; i++)
	{
		// スプライト描画
		spriteDatas_[i]->Draw();
	}

	spriteIndex_ = 0;
}