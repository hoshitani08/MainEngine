#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <memory>

#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include "FbxObject3d.h"
#include "ShaderManager.h"

class Object3d
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

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;  //ビュープロジェクション行列
		XMMATRIX world;     //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標(ワールド座標)
		float pad1;         //パディング
		XMFLOAT4 color;     // 色
	};

private: // 定数

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// 静的破棄
	static void StaticFinalize();
	// 3Dオブジェクト生成
	static std::unique_ptr<Object3d> Create(Model* model = nullptr);
	// カメラのセット
	static void SetCamera(Camera* camera) { Object3d::camera_ = camera; }
	//ライトのセット
	static void SetLight(LightGroup* light) { Object3d::light_ = light; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// カメラ
	static Camera* camera_;
	//ライト
	static LightGroup* light_;

public: // メンバ関数
	// コンストラクタ
	Object3d() = default;
	// デストラクタ
	virtual ~Object3d();
	// 初期化
	virtual bool Initialize();
	// 毎フレーム処理
	virtual void Update();
	// 描画
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	// 行列の更新
	void UpdateWorldMatrix();
	// 座標の取得
	const XMFLOAT3& GetPosition() { return position_; }
	// 座標の設定
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	// X,Y,Z軸回りの取得
	const XMFLOAT3& GetRotation() { return rotation_; }
	// X,Y,Z軸回りの設定
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	// スケールの取得
	const XMFLOAT3& GetScale() { return scale_; }
	// スケールの設定
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	// モデルの設定
	void SetModel(Model* model) { model_ = model; };
	// ビルボードの設定
	void SetBillboard(bool isBillboard) { isBillboard_ = isBillboard; }
	// ワールド行列の取得
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	// コライダーのセット
	void SetCollider(BaseCollider* collider);
	// 衝突時コールバック関数
	virtual void OnCollision(const CollisionInfo& info) {}
	// ワールド座標を取得
	XMFLOAT3 GetWorldPosition();
	// モデルを取得
	inline Model* GetModel() { return model_; }
	// 色の設定
	void SetColor(XMFLOAT4 _color) { color_ = _color; }
	// 親オブジェクトの設定
	void SetParent(Object3d* parent) { parent_ = parent; }
	void SetParent(FbxObject3d* parent) { fbxParent_ = parent; }
	// 取得するボーンの名前の設定
	void SetBoneName(std::string boneName) { boneName_ = boneName; }
	// プリミティブ形状の設定
	void SetPrimitiveType(ShaderManager::Type type) { type_ = type; }

protected: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0_; // 定数バッファ
	// 色
	XMFLOAT4 color_ = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_ = {};
	// 親オブジェクト
	Object3d* parent_ = nullptr;
	FbxObject3d* fbxParent_ = nullptr;
	// モデル
	Model* model_ = nullptr;
	// ビルボード
	bool isBillboard_ = false;
	//クラス名(デバック用)
	const char* name_ = nullptr;
	// コライダー
	BaseCollider* collider_ = nullptr;
	// ボーンのファイル名
	std::string boneName_;
	//
	ShaderManager::Type type_ = ShaderManager::Type::Triangle;
};