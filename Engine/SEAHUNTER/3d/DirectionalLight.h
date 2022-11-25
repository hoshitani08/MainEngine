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
	/// <summary>
	/// ライト方向を設定
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightdir) { lightDir_ = DirectX::XMVector3Normalize(lightdir); }
	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetLightDir() { return lightDir_; }
	/// <summary>
	/// ライト色を設定
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	void SetLightColor(const XMFLOAT3& lightcolor) { lightColor_ = lightcolor; }
	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const XMFLOAT3& GetLightColor() { return lightColor_; }
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
	//ライト方向（単位ベクトル）
	XMVECTOR lightDir_ = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightColor_ = { 1,1,1 };
	//有効フラグ
	bool active_ = false;
};