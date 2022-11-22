#pragma once

#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#include "Sprite.h"

class DebugText final
{
private: // エイリアス
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int MAX_CHAR_COUNT = 256; // 最大文字数
	static const int FONT_WIDTH = 9; // フォント画像内1文字分の横幅
	static const int FONT_HEIGHT = 18; // フォント画像内1文字分の縦幅
	static const int FONT_LINECOUNT = 14; // フォント画像内1行分の文字数

public:// 静的メンバ関数
	static DebugText* GetInstance();

public:// メンバ関数

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void VariablePrint(float x, float y, const std::string& text, float i, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

	DebugText(const DebugText&) = delete;
	DebugText& operator=(const DebugText&) = delete;

private:
	DebugText();
	~DebugText();

private:
	// スプライトデータの配列
	std::unique_ptr<Sprite> spriteDatas_[MAX_CHAR_COUNT];
	// スプライトデータ配列の添え字番号
	int spriteIndex_ = 0;

	XMVECTOR variable_ = {};
};