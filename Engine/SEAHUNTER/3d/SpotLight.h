#pragma once

#include <DirectXMath.h>

class SpotLight
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
		XMVECTOR lightv;
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		float pad3;
		XMFLOAT2 lightFactorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数
	/// <summary>
	/// ライト方向を設定
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	inline void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	/// <summary>
	/// ライト座標を設定
	/// </summary>
	/// <param name="lightpos">ライト座標</param>
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightPos_ = lightpos; }
	/// <summary>
	/// ライト座標を取得
	/// </summary>
	/// <returns>ライト座標</returns>
	inline const XMFLOAT3& GetLightPos() { return lightPos_; }
	/// <summary>
	/// ライト色を設定
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	/// <summary>
	/// ライト距離減衰係数を設定
	/// </summary>
	/// <param name="lightAtten">ライト距離減衰係数</param>
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; }
	/// <summary>
	/// ライト距離減衰係数を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; }
	/// <summary>
	/// ライト減衰角度を設定
	/// </summary>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	/// <summary>
	/// ライト減衰角度を取得
	/// </summary>
	/// <returns>ライト減衰角度</returns>
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos_; }
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
	// ライト方向（単位ベクトル）
	XMVECTOR lightDir_ = { 1,0,0,0 };
	// ライト座標（ワールド座標系）
	XMFLOAT3 lightPos_ = { 0,0,0 };
	// ライト色
	XMFLOAT3 lightColor_ = { 1,1,1 };
	// ライト距離減衰係数
	XMFLOAT3 lightAtten_ = { 1.0f, 1.0f, 1.0f };
	// ライト減衰角度
	XMFLOAT2 lightFactorAngleCos_ = { 0.2f, 0.5f };
	// 有効フラグ
	bool active_ = false;
};