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
	static const float clearColor[4];
	// 頂点数
	static const int vertNum = 4;

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
	//コンストラクタ
	PostEffect() = default;
	// 初期化
	void Initialize(ID3D12Device* device);
	//描画コマンドの発行
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//シーン描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//シーン描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
// D3D12デバイス
	ID3D12Device* device = nullptr;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[2];
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 色 (RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 明度
	float brightnessColor = 1.0f;
};

