#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Model.h"

class ObjFactory final
{
public: // サブクラス
	struct ModelData
	{
		std::string fName; // ファイル名
		std::unique_ptr<Model> model; // OBJモデル
	};

public:
	// ルートパス
	static const std::string BASE_DIRECTORY;

private:
	ObjFactory() = default;
	~ObjFactory() = default;

public:
	ObjFactory(const ObjFactory& objFactory) = delete;
	ObjFactory& operator=(const ObjFactory& objFactory) = delete;

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
	/// <param name="smoothing">エッジ平滑化フラグ</param>
	void LoadModel(std::string fName, bool smoothing = false);
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <param name="fName">ファイル名</param>
	/// <returns>検索したモデル</returns>
	Model* GetModel(std::string fName);
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ObjFactory* GetInstance();

private:
	// モデルデータ
	std::vector<ModelData> modelData_;
};