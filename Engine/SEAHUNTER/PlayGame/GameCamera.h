#pragma once

#include "Camera.h"
#include "Hunter.h"
#include "Ease.h"

class GameCamera
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // 定数
	const XMFLOAT2 RESTRICTION_ANGLE = { 360.0f, 80.0f };

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameCamera();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameCamera();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// カメラのアングル
	/// </summary>
	void CameraAngle(XMFLOAT2 angle);
	/// <summary>
	/// 開始時のカメラ演出
	/// </summary>
	void StratCameraMove();
	/// <summary>
	/// ゲーム中のカメラ制御
	/// </summary>
	void GamePlayCameraMove();
	/// <summary>
	/// 終了時のカメラ演出
	/// </summary>
	void EndCameraMove();
	/// <summary>
	/// カメラリセット
	/// </summary>
	void CameraReset();
	/// <summary>
	/// カメラのイージングが終わったか
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCameraMoveEnd() { return easeCamera->GetEndFlag(); }
	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamerapoint() { return camera_.get(); }
	/// <summary>
	/// カメラアングルの取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 GetCameraAngle() { return angle_; }
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="hunter">プレイヤー</param>
	void SetHunter(Hunter* hunter) { hunter_ = hunter; }
	/// <summary>
	/// イージングデータのリセット
	/// </summary>
	void EaseDataReset();

private: // メンバ変数
	//カメラ
	std::unique_ptr<Camera> camera_;
	// イージングの進行度用
	std::unique_ptr<EaseData> easeCamera;
	//ハンター
	Hunter* hunter_ = nullptr;
	// 角度
	XMFLOAT2 angle_ = { 0.0f, 0.0f };
	// 方向のリセットフラグ
	bool cameraResetFlag = false;
};

