#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	// 平行光源の数
	static const int DIR_LIGHT_NUM = 3;
	// 点光源の数
	static const int POINT_LIGHT_NUM = 3;
	// スポットライトの数
	static const int SPOT_LIGHT_NUM = 3;
	// 丸影の数
	static const int CIRCLE_SHADOW_NUM = 1;

public: // サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
		// 点光源用
		PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
		// スポットライト用
		SpotLight::ConstBufferData spotLights[SPOT_LIGHT_NUM];
		// 丸影用
		CircleShadow::ConstBufferData circleShadows[CIRCLE_SHADOW_NUM];
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<LightGroup> Create();

private: // 静的メンバ変数
	//デバイス
	static ID3D12Device* device_;

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LightGroup();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートハンドル</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();
	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();
	/// <summary>
	/// 環境光のライト色を設定
	/// </summary>
	/// <param name="color">ライト色</param>
	void SetAmbientColor(const XMFLOAT3& color);
	/// <summary>
	/// 平行光源の有効フラグを設定
	/// </summary>
	/// <param name="index"></param>
	/// <param name="active"></param>
	void SetDirLightActive(int index, bool active);
	/// <summary>
	/// 平行光源のライト方向を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// 平行光源のライト色を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// 点光源の有効フラグを設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetPointLightActive(int index, bool active);
	/// <summary>
	/// 点光源のライト座標を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightpos">ライト座標</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);
	/// <summary>
	/// 点光源のライト色を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// 点光源のライト距離減衰係数を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">ライト距離減衰係数</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// スポットライトの有効フラグを設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetSpotLightActive(int index, bool active);
	/// <summary>
	/// スポットライトのライト方向を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// スポットライトのライト座標を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightpos">ライト座標</param>
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	/// <summary>
	/// スポットライトのライト色を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	/// <summary>
	/// スポットライトのライト距離減衰係数を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">ライト距離減衰係数</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// スポットライトのライト減衰角度を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);
	/// <summary>
	/// 丸影の有効フラグを設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetCircleShadowActive(int index, bool active);
	/// <summary>
	/// 丸影のキャスター座標を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="casterPos">キャスター座標</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);
	/// <summary>
	/// 丸影の方向を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">方向</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);
	/// <summary>
	/// 丸影のキャスターとライトの距離を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	/// <summary>
	/// 丸影の距離減衰係数を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">距離減衰係数</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);
	/// <summary>
	/// 丸影の減衰角度を設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

private: // メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	//環境光の色
	XMFLOAT3 ambientColor_ = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights_[DIR_LIGHT_NUM];
	//点光源の配列
	PointLight pointLights_[POINT_LIGHT_NUM];
	// スポットライトの配列
	SpotLight spotLights_[SPOT_LIGHT_NUM];
	// 丸影の配列
	CircleShadow circleShadows_[CIRCLE_SHADOW_NUM];
	//ダーティフラグ
	bool dirty_ = false;
};