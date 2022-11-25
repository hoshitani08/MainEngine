#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Mesh.h"

class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// OBJファイルからメッシュ生成
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <param name="modelname">エッジ平滑化フラグ</param>
	/// <returns>生成されたモデル</returns>
	static std::unique_ptr<Model> CreateFromObject(const std::string& text, bool smoothing = false);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;

private:// 静的メンバ関数
	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath">読み込みディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// マテリアル登録
	/// </summary>
	/// <param name="material">マテリアル</param>
	void AddMaterial(Material* material);
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void CreateDescriptorHeap();
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="text">モデル名</param>
	/// <param name="smoothing">エッジ平滑化フラグ</param>
	void Initialize(const std::string& text, bool smoothing);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// アルファ値を設定
	/// </summary>
	/// <param name="alpha">アルファ値</param>
	void SetAlpha(float alpha);
	/// <summary>
	/// メッシュコンテナを取得
	/// </summary>
	/// <returns>メッシュコンテナ</returns>
	inline const std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return meshes_; }

private: // メンバ変数
	// 名前
	std::string name_;
	// メッシュコンテナ
	std::vector<std::unique_ptr<Mesh>> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, std::unique_ptr<Material>> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;
};