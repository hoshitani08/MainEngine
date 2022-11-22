#pragma once

#include <DirectXMath.h>

class Camera
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// コンストラクタ
	Camera(int window_width, int window_height);
	// デストラクタ
	~Camera() = default;
	// 毎フレーム更新
	void Update();
	// ビュー行列を更新
	void UpdateViewMatrix();
	// 射影行列を更新
	void UpdateProjectionMatrix();
	// ビュー行列の取得
	const XMMATRIX& GetViewMatrix() { return matView_; }
	// 射影行列の取得
	const XMMATRIX& GetProjectionMatrix() { return matProjection_; }
	// ビュー射影行列の取得
	const XMMATRIX& GetViewProjectionMatrix() { return matViewProjection_; }
	// ビルボード行列の取得
	const XMMATRIX& GetBillboardMatrix() { return matBillboard_; }
	// 視点座標の取得
	const XMFLOAT3& GetEye() { return eye_; }
	// 視点座標の設定
	void SetEye(XMFLOAT3 eye) { eye_ = eye; viewDirty_ = true; }
	// 注視点座標の取得
	const XMFLOAT3& GetTarget() { return target_; }
	// 注視点座標の設定
	void SetTarget(XMFLOAT3 target) { target_ = target; viewDirty_ = true; }
	// ベクトルの取得
	const XMFLOAT3& GetUp() { return up_; }
	// ベクトルの設定
	void SetUp(XMFLOAT3 up) { up_ = up; viewDirty_ = true; }
	// ベクトルによる移動
	void CameraMoveVector(const XMFLOAT3& move);
	void CameraMoveEyeVector(const XMFLOAT3& move);
	void CameraMoveTargetVector(const XMFLOAT3& move);

protected: // メンバ変数
	// ビュー行列
	XMMATRIX matView_ = DirectX::XMMatrixIdentity();
	// ビルボード行列
	XMMATRIX matBillboard_ = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY_ = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection_ = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	XMMATRIX matViewProjection_ = DirectX::XMMatrixIdentity();
	// ビュー行列ダーティフラグ
	bool viewDirty_ = false;
	// 射影行列ダーティフラグ
	bool projectionDirty_ = false;
	// 視点座標
	XMFLOAT3 eye_ = { 0, 0, -50 };
	// 注視点座標
	XMFLOAT3 target_ = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up_ = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio_ = 1.0f;
};