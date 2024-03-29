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
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	/// <summary>
	/// 静的破棄
	/// </summary>
	static void StaticFinalize();
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>生成されたオブジェクト</returns>
	static std::unique_ptr<Object3d> Create(Model* model = nullptr);
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) { Object3d::camera_ = camera; }
	/// <summary>
	/// ライトの設定
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(LightGroup* light) { Object3d::light_ = light; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// カメラ
	static Camera* camera_;
	//ライト
	static LightGroup* light_;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateWorldMatrix();
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position_; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	/// <summary>
	/// X,Y,Z軸回りの取得
	/// </summary>
	/// <returns>X,Y,Z軸回りの回転</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }
	/// <summary>
	/// X,Y,Z軸回りの設定
	/// </summary>
	/// <param name="rotation">X,Y,Z軸回りの回転</param>
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns>スケール</returns>
	const XMFLOAT3& GetScale() { return scale_; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { model_ = model; };
	/// <summary>
	/// ビルボードの設定
	/// </summary>
	/// <param name="isBillboard">ビルボードさせるか</param>
	void SetBillboard(bool isBillboard) { isBillboard_ = isBillboard; }
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	/// <summary>
	/// コライダーの設定
	/// </summary>
	/// <param name="collider">衝突判定</param>
	void SetCollider(BaseCollider* collider);
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	XMFLOAT3 GetWorldPosition();
	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル</returns>
	inline Model* GetModel() { return model_; }
	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="_color">色</param>
	void SetColor(XMFLOAT4 _color) { color_ = _color; }
	/// <summary>
	/// 親オブジェクトの設定
	/// </summary>
	/// <param name="parent">親オブジェクト</param>
	void SetParent(Object3d* parent) { parent_ = parent; }
	void SetParent(FbxObject3d* parent) { fbxParent_ = parent; }
	/// <summary>
	/// 取得するボーンの名前の設定
	/// </summary>
	/// <param name="boneName">ボーン名</param>
	void SetBoneName(std::string boneName) { boneName_ = boneName; }
	/// <summary>
	/// プリミティブ形状の設定
	/// </summary>
	/// <param name="type"></param>
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