#pragma once

#include <DirectXMath.h>

class CircleShadow
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数
	/// <summary>
	/// 方向を設定
	/// </summary>
	/// <param name="dir"></param>
	inline void SetDir(const XMVECTOR& dir) { dir_ = DirectX::XMVector3Normalize(dir); }
	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetDir() { return dir_; }
	/// <summary>
	/// キャスター座標を設定
	/// </summary>
	/// <param name="casterPos">キャスター座標</param>
	inline void SetCasterPos(const XMFLOAT3& casterPos) { casterPos_ = casterPos; }
	/// <summary>
	/// キャスター座標を取得
	/// </summary>
	/// <returns>キャスター座標</returns>
	inline const XMFLOAT3& GetCasterPos() { return casterPos_; }
	/// <summary>
	/// キャスターとライトの距離を設定
	/// </summary>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	inline void SetDistanceCasterLight(float distanceCasterLight) { distanceCasterLight_ = distanceCasterLight; }
	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns>キャスターとライトの距離</returns>
	inline float GetDistanceCasterLight() { return distanceCasterLight_; }
	/// <summary>
	/// 距離減衰係数を設定
	/// </summary>
	/// <param name="atten">距離減衰係数</param>
	inline void SetAtten(const XMFLOAT3& atten) { atten_ = atten; }
	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>距離減衰係数</returns>
	inline const XMFLOAT3& GetAtten() { return atten_; }
	/// <summary>
	/// 減衰角度を設定
	/// </summary>
	/// <param name="factorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	inline void SetFactorAngle(const XMFLOAT2& factorAngle)
	{
		factorAngleCos_.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		factorAngleCos_.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	/// <summary>
	/// 減衰角度を取得
	/// </summary>
	/// <returns>距離減衰係数</returns>
	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos_; }
	/// <summary>
	/// 有効フラグを設定
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { active_ = active; }
	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active_; }

private: // メンバ変数
	// 方向（単位ベクトル）
	XMVECTOR dir_ = { 1,0,0,0 };
	// キャスターとライトの距離
	float distanceCasterLight_ = 100.0f;
	// キャスター座標（ワールド座標系）
	XMFLOAT3 casterPos_ = { 0,0,0 };
	// 距離減衰係数
	XMFLOAT3 atten_ = { 0.5f, 0.6f, 0.0f };
	// 減衰角度
	XMFLOAT2 factorAngleCos_ = { 0.2f, 0.5f };
	// 有効フラグ
	bool active_ = false;
};