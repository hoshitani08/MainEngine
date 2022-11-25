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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">表示座標X</param>
	/// <param name="window_height">表示座標Y</param>
	Camera(int window_width, int window_height);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera() = default;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();
	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateViewMatrix();
	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateProjectionMatrix();
	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	const XMMATRIX& GetViewMatrix() { return matView_; }
	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	const XMMATRIX& GetProjectionMatrix() { return matProjection_; }
	/// <summary>
	/// ビュー射影行列の取得
	/// </summary>
	/// <returns>ビュー射影行列</returns>
	const XMMATRIX& GetViewProjectionMatrix() { return matViewProjection_; }
	/// <summary>
	/// ビルボード行列の取得
	/// </summary>
	/// <returns>ビルボード行列</returns>
	const XMMATRIX& GetBillboardMatrix() { return matBillboard_; }
	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>視点座標</returns>
	const XMFLOAT3& GetEye() { return eye_; }
	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="eye">視点座標</param>
	void SetEye(XMFLOAT3 eye) { eye_ = eye; viewDirty_ = true; }
	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>注視点座標</returns>
	const XMFLOAT3& GetTarget() { return target_; }
	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="target">注視点座標</param>
	void SetTarget(XMFLOAT3 target) { target_ = target; viewDirty_ = true; }
	/// <summary>
	/// ベクトルの取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const XMFLOAT3& GetUp() { return up_; }
	/// <summary>
	/// ベクトルの設定
	/// </summary>
	/// <param name="up">ベクトル</param>
	void SetUp(XMFLOAT3 up) { up_ = up; viewDirty_ = true; }
	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">ベクトル</param>
	void CameraMoveVector(const XMFLOAT3& move);
	/// <summary>
	/// ベクトルによる移動(Eyeのみ)
	/// </summary>
	/// <param name="move">ベクトル</param>
	void CameraMoveEyeVector(const XMFLOAT3& move);
	/// <summary>
	/// ベクトルによる移動(Targetのみ)
	/// </summary>
	/// <param name="move">ベクトル</param>
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