#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PostEffect
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const float CLEAR_COLOR[4];
	// 頂点数
	static const int VERT_NUM = 4;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		float brightnessColor; // 明度
		XMMATRIX mat; // ３Ｄ変換行列
	};

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
// D3D12デバイス
	ID3D12Device* device_ = nullptr;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_[2];
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff_;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 色 (RGBA)
	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// 明度
	float brightnessColor_ = 1.0f;
};

