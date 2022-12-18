#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>

class Sprite
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
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列
	};

private: // 静的定数
	// テクスチャの最大枚数
	static const int SRV_COUNT = 512;
	// 頂点数
	static const int VERT_NUM = 4;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns></returns>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	/// <summary>
	/// 静的破棄
	/// </summary>
	static void StaticFinalize();
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texnumber">テクスチャハンドル</param>
	/// <param name="filename">ルートパス</param>
	/// <returns></returns>
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);
	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();
	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texNumber">テクスチャハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="color">色</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	/// <returns>生成されたスプライト</returns>
	static std::unique_ptr<Sprite> Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// 射影行列
	static XMMATRIX matProjection_;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap_;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff_[SRV_COUNT];

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="texNumber">テクスチャハンドル</param>
	/// <param name="position">座標</param>
	/// <param name="size">サイズ</param>
	/// <param name="color">色</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();
	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns>角度</returns>
	const float& GetRotation() { return rotation_; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度</param>
	void SetRotation(float rotation);
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT2& GetPosition() { return position_; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT2 position);
	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(XMFLOAT2 size);
	/// <summary>
	/// サイズの取得
	/// </summary>
	/// <returns>サイズ</returns>
	const XMFLOAT2& GetSize() { return size_; }
	/// <summary>
	/// 倍率の設定
	/// </summary>
	/// <param name="scale">倍率</param>
	void SetScale(float scale);
	/// <summary>
	/// 倍率の取得
	/// </summary>
	/// <returns>倍率</returns>
	float GetScale() { return scale_; }
	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="_color">色</param>
	void SetColor(XMFLOAT4 _color);
	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorpoint">アンカーポイント</param>
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	/// <summary>
	/// 左右反転の設定
	/// </summary>
	/// <param name="isFlipX">左右反転</param>
	void SetIsFlipX(bool isFlipX);
	/// <summary>
	/// 上下反転の設定
	/// </summary>
	/// <param name="isFlipY">上下反転</param>
	void SetIsFlipY(bool isFlipY);
	/// <summary>
	/// テクスチャ範囲設定
	/// </summary>
	/// <param name="texBase">テクスチャ左上座標</param>
	/// <param name="texSize">テクスチャサイズ</param>
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// テクスチャ番号
	UINT texNumber_ = 0;
	// Z軸回りの回転角
	float rotation_ = 0.0f;
	// 座標
	XMFLOAT2 position_ = {};
	// スプライト幅、高さ
	XMFLOAT2 size_ = { 100.0f, 100.0f };
	XMFLOAT2 defSize_ = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorPoint_ = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld_ = {};
	// 色
	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;
	// テクスチャ始点
	XMFLOAT2 texBase_ = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize_ = { 100.0f, 100.0f };
	//スケール
	float scale_ = 1.0f;

private: // メンバ関数
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();
};