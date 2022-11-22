#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <unordered_map>

#include "Material.h"

class Mesh
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;

public: // メンバ関数

	~Mesh();
	// 名前を取得
	const std::string& GetName() { return name_; }
	// 名前をセット
	void SetName(const std::string& name);
	// 頂点データの追加
	void AddVertex(const VertexPosNormalUv& vertex);
	// 頂点インデックスの追加
	void AddIndex(unsigned short index);
	// 頂点データの数を取得
	inline size_t GetVertexCount() { return vertices_.size(); }
	// マテリアルの取得
	Material* GetMaterial() { return material_; }
	// マテリアルの割り当て
	void SetMaterial(Material* material);
	// バッファの生成
	void CreateBuffers();
	// 頂点バッファ取得
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }
	// インデックスバッファ取得
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//エッジ平滑化データの追加
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	//平滑化された頂点法線の計算
	void CalculateSmoothedVetexNormals();
	// 頂点配列を取得
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }
	// インデックス配列を取得
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

private: // メンバ変数
	// 名前
	std::string name_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// マテリアル
	Material* material_ = nullptr;
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
};