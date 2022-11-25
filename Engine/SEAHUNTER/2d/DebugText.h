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
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static DebugText* GetInstance();

public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texnumber">テクスチャハンドル</param>
	void Initialize(UINT texnumber);
	/// <summary>
	/// 文字列追加
	/// </summary>
	/// <param name="text">文字列</param>
	/// <param name="x">表示座標X</param>
	/// <param name="y">表示座標Y</param>
	/// <param name="size">倍率</param>
	void Print(const std::string& text, float x, float y, float size);
	/// <summary>
	/// 文字列+変数追加
	/// </summary>
	/// <param name="x">表示座標X</param>
	/// <param name="y">表示座標Y</param>
	/// <param name="text">文字列</param>
	/// <param name="i">変数</param>
	/// <param name="size">倍率</param>
	void VariablePrint(float x, float y, const std::string& text, float i, float size);
	/// <summary>
	/// 描画フラッシュ
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
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
	// 変数を描画するための保存
	XMVECTOR variable_ = {};
};