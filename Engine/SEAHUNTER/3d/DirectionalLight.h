#pragma once

#include <DirectXMath.h>

class DirectionalLight
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightColor;
		unsigned int active;
	};

public: // メンバ関数
	//ライト方向をセット
	void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	//ライト方向を取得
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	//ライト色をセット
	void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	//ライト色を取得
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
	//有効フラグをセット
	inline void SetActive(bool active) { active_ = active; }
	//有効チェック
	inline bool IsActive() { return active_; }

private: // メンバ変数
	//ライト方向（単位ベクトル）
	XMVECTOR lightDir_ = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightColor_ = { 1,1,1 };
	//有効フラグ
	bool active_ = false;
};