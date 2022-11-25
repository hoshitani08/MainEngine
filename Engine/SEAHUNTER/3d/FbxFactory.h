#pragma once

#include <string>
#include <vector>
#include <memory>

#include "FbxModel.h"
#include "FbxLoader.h"

class FbxFactory final
{
public: // サブクラス
	struct ModelData
	{
		std::string fName; // ファイル名
		std::unique_ptr<FbxModel> fbxModel; // FBXモデル
	};

public:
	// ルートパス
	static const std::string BASE_DIRECTORY;

private:
	FbxFactory() = default;
	~FbxFactory() = default;

public:
	FbxFactory(const FbxFactory & fbxFactory) = delete;
	FbxFactory& operator=(const FbxFactory & fbxFactory) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// モデルのロード
	/// </summary>
	/// <param name="fName">ファイル名</param>
	void LoadModel(std::string fName);
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <param name="fName">ファイル名</param>
	/// <returns>検索したモデル</returns>
	FbxModel* GetModel(std::string fName);
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static FbxFactory* GetInstance();

private:
	// モデルデータ
	std::vector<ModelData> modelData_;
};