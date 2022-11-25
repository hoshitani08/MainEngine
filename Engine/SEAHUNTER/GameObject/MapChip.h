#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

class MapChip final
{
private: // エイリアス
	// std::を省略
	using string = std::string;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;

public: // サブクラス
	// アニメーション用データ構造体
	struct MapChipData
	{
		string mapName = ""; // .csv名
		std::vector<int> mapCsvNumber = {}; // 格納コンテナ
		int mapChipMaxX = 0; // 最大の横幅
		int mapChipMaxY = 0; // 最大の縦幅
	};

private:
	MapChip();
	~MapChip();

public:
	MapChip(const MapChip& mapChip) = delete;
	MapChip& operator=(const MapChip& mapChip) = delete;
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static MapChip* GetInstance();

public:
	// ルートパス
	static const string baseDirectory;

public:
	/// <summary>
	/// CSVファイル読み込み
	/// </summary>
	/// <param name="mapChipMaxX">最大の横幅</param>
	/// <param name="mapChipMaxY">最大の縦幅</param>
	/// <param name="fName">ファイル名</param>
	void CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName);
	/// <summary>
	/// マップナンバー指定
	/// </summary>
	/// <param name="x">横</param>
	/// <param name="y">縦</param>
	/// <param name="fName">ファイル名</param>
	/// <param name="mapChipSize"></param>
	/// <returns></returns>
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);
	/// <summary>
	/// 最大の横幅を取得
	/// </summary>
	/// <param name="fName">ファイル名</param>
	/// <returns>最大の横幅</returns>
	const int& GetMapChipMaxX(std::string fName);
	/// <summary>
	/// 最大の縦幅を取得
	/// </summary>
	/// <param name="fName">ファイル名</param>
	/// <returns>最大の縦幅</returns>
	const int& GetMapChipMaxY(std::string fName);

private:
	std::vector<MapChipData> mapData_;
};