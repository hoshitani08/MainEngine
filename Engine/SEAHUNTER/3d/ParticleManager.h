#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <memory>

#include "Camera.h"

class ParticleManager
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
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
		XMFLOAT4 color; // 色(RGBA)
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat; // ビュープロジェクション行列
		XMMATRIX matBillboard; // ビルボード行列
	};

	//パーティクル1粒
	class Particle
	{
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int numFrame = 0;
		// スケール
		float scale = 1.0f;
		// 初期値
		float startScale = 1.0f;
		// 最終値
		float endScale = 0.0f;
		// 色(RGBA)
		XMFLOAT4 color = { 1, 1, 1, 1 };
		// 色(RGBA)初期値
		XMFLOAT4 startColor = {};
		// 色(RGBA)最終値
		XMFLOAT4 endColor = {};
	};

private: // 定数
	static const int VERTEX_COUNT = 65536; // 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>
	/// <param name="fName">ファイル名</param>
	/// <returns></returns>
	static std::unique_ptr<ParticleManager> Create(ID3D12Device* device, Camera* camera, std::wstring fName = L"effect1");

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleManager();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fName">ファイル名</param>
	void Initialize(std::wstring fName);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="startScale">初期値の倍率</param>
	/// <param name="endScale">最終値の倍率</param>
	/// <param name="startColor">色(RGBA)初期値</param>
	/// <param name="endColor">色(RGBA)最終値</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		float startScale, float endScale, XMFLOAT4 startColor, XMFLOAT4 endColor);
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="fName"></param>
	void LoadTexture(std::wstring fName);
	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

private: // 静的メンバ変数
	// デバイス
	ID3D12Device* device_ = nullptr;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize_ = 0u;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	//パーティクル配列
	std::forward_list<Particle> particles_;
	// カメラ
	Camera* camera_ = nullptr;

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>
	ParticleManager(ID3D12Device* device, Camera* camera);
};