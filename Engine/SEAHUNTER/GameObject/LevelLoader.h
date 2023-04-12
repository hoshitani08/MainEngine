#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <memory>

// レベルデータ
struct LevelData
{
	struct ObjectData
	{
		// ファイル名
		std::string fileName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};
	struct ItemData
	{
		// ファイル名
		std::string fileName;
		// 平行移動
		int quantity;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
	std::vector<ItemData> items;
};

class LevelLoader
{
public: // 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public: // メンバ関数
	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static std::unique_ptr<LevelData> LoadFile(const std::string& fileName);
};

