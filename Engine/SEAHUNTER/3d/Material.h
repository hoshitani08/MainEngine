#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; //アンビエント係数
		float pad1; //パディング
		XMFLOAT3 diffuse; //ディフューズ係数
		float pad2; //パディング
		XMFLOAT3 specular; //スペキュラー係数
		float alpha; //アルファ
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);
	//マテリアル生成
	static Material* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

public: // メンバ関数

	~Material();
	// 定数バッファの取得
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }
	// テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	// 更新
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV_; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV_; }

private: // メンバ関数
	//コンストラクタ
	Material()
	{
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}
	// 初期化
	void Initialize();
	// 定数バッファの生成
	void CreateConstantBuffer();

public: //メンバ変数
	std::string name_; //マテリアル名
	XMFLOAT3 ambient_; //アンビエント影響度
	XMFLOAT3 diffuse_; //ディフューズ影響度
	XMFLOAT3 specular_; //スペキュラー影響度
	float alpha_; //アルファ
	std::string textureFilename_; //テクスチャファイル名
};

