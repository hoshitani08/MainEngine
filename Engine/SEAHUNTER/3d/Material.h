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
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// マテリアル生成
	/// </summary>
	/// <returns>生成されたマテリアル</returns>
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
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Material();
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath">読み込みディレクトリパス</param>
	/// <param name="cpuHandle"></param>
	/// <param name="gpuHandle"></param>
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// シェーダリソースビューのハンドル(CPU)の取得
	/// </summary>
	/// <returns>シェーダリソースビューのハンドル(CPU)</returns>
	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV_; }
	/// <summary>
	/// シェーダリソースビューのハンドル(GPU)の取得
	/// </summary>
	/// <returns>シェーダリソースビューのハンドル(GPU)</returns>
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV_; }

private: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Material()
	{
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstantBuffer();

public: //メンバ変数
	std::string name_; //マテリアル名
	XMFLOAT3 ambient_; //アンビエント影響度
	XMFLOAT3 diffuse_; //ディフューズ影響度
	XMFLOAT3 specular_; //スペキュラー影響度
	float alpha_; //アルファ
	std::string textureFilename_; //テクスチャファイル名
};

