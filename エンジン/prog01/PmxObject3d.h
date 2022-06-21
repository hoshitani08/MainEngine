#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "PmxModel.h"
#include "Camera.h"
#include "LightGroup.h"

class PmxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// texturedModelシェーダーの定数バッファデータ型
	struct TexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// notTexturedModelシェーダーの定数バッファデータ型
	struct NotTexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMVECTOR diffuseColor;
		XMVECTOR ambientColor;
	};

	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferData
	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// 3Dオブジェクト生成
	static std::unique_ptr<PmxObject3d> Create(PmxModel* model = nullptr);
	// setter
	static void SetDevice(ID3D12Device* device) { PmxObject3d::device = device; }
	static void SetCamera(Camera* camera) { PmxObject3d::camera = camera; }
	static void SetLight(LightGroup* light) { PmxObject3d::light = light; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// カメラ
	static Camera* camera;
	//ライト
	static LightGroup* light;

public: // メンバ関数
	// 初期化
	bool Initialize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// 行列の更新
	void UpdateWorldMatrix();
	//モデルを設定
	void SetModel(PmxModel* _model) { model = _model; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }
	// 座標の設定
	void SetPosition(XMFLOAT3 _position) { position = _position; }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return rotation; }
	// X,Y,Z軸回りの設定
	void SetRotation(XMFLOAT3 _rotation) { rotation = _rotation; }
	// スケールの取得
	const XMFLOAT3& GetScale() { return scale; }
	// スケールの設定
	void SetScale(XMFLOAT3 _scale) { scale = _scale; }

protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	PmxModel* model = nullptr;
	//クラス名(デバック用)
	const char* name = nullptr;
};

