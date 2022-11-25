#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

#include "fbxsdk.h"
#include "FbxModel.h"

using namespace DirectX;

class FbxLoader
{
private: // エイリアス
	// std::を省略
	using string = std::string;
public: // 定数
	// モデル格納ルートパス
	static const string BASE_DIRECTORY;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string DEFAULT_TEXTURE_FILE_NAME;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static FbxLoader* GetInstance();
	//FBXの行列をXMMatrixに変換
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();
	/// <summary>
	/// ファイルからFBXモデル読込
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <returns>生成されたモデル</returns>
	std::unique_ptr<FbxModel> LoadModelFromFile(const string& modelName);

private:
	// D3D12デバイス
	ID3D12Device* device_ = nullptr;
	// FBXマネージャ
	FbxManager* fbxManager_ = nullptr;
	// FBXインポータ
	FbxImporter* fbxImporter_ = nullptr;
	// コントロールポイントのデータ
	std::vector<std::vector<int>> controlPointsData_;

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);
	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
	/// <summary>
	/// 頂点座標読み取り、面情報読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxNode"></param>
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texData"></param>
	/// <param name="fullpath"></param>
	void LoadTexture(TextureData* texData, const std::string& fullpath);
	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);
	/// <summary>
	/// ディレクトリを含んだファイルパスからファイル名を抽出する
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::string ExtractFileName(const std::string& path);
	/// <summary>
	/// 3頂点からローカル座標とUV座標からU軸(angent)とV軸(Binormal)を求める
	/// </summary>
	/// <param name="model"></param>
	void BuildTangentAndBiNormalImp(FbxModel* model);

	const XMFLOAT3& Add(XMFLOAT3 m, XMFLOAT3 k);
	const XMFLOAT3& Normalize(XMFLOAT3 m);
	const XMFLOAT3& Cross(XMFLOAT3 m, XMFLOAT3 k);
};