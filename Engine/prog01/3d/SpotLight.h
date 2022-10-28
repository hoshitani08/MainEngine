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
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		float pad3;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数
	// ライト方向をセット
	inline void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	// ライト方向を取得
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	// ライト座標をセット
	inline void SetLightPos(const XMFLOAT3& lightpos) { lightPos_ = lightpos; }
	// ライト座標を取得
	inline const XMFLOAT3& GetLightPos() { return lightPos_; }
	// ライト色をセット
	inline void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	// ライト色を取得
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	// ライト距離減衰係数をセット
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { lightAtten_ = lightAtten; }
	// ライト距離減衰係数を取得
	inline const XMFLOAT3& GetLightAtten() { return lightAtten_; }
	// ライト減衰角度をセット
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle)
	{
		lightFactorAngleCos_.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		lightFactorAngleCos_.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	// ライト減衰角度を取得
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos_; }
	// 有効フラグをセット
	inline void SetActive(bool active) { active_ = active; }
	// 有効チェック
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