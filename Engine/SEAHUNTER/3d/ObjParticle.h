#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include <vector>

#include "FbxObject3d.h"
#include "Object3d.h"

class ObjParticle
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

	//パーティクル1粒
	class ObjectParticle
	{
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int numFrame = 0;
		// 初期値
		XMFLOAT3 startScale = { 1.0f, 1.0f, 1.0f };
		// 最終値
		XMFLOAT3 endScale = { 0.0f, 0.0f, 0.0f };
		// 色(RGBA)初期値
		XMFLOAT4 startColor = {};
		// 色(RGBA)最終値
		XMFLOAT4 endColor = {};
	};

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjParticle();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="startScale">初期値の倍率</param>
	/// <param name="endScale">最終値の倍率</param>
	/// <param name="startColor">色(RGBA)初期値</param>
	/// <param name="endColor">色(RGBA)最終値</param>
	/// <param name="model">モデル</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		XMFLOAT3 startScale, XMFLOAT3 endScale, XMFLOAT4 startColor, XMFLOAT4 endColor, Model* model);
	/// <summary>
	/// 親オブジェクトの設定
	/// </summary>
	/// <param name="parent">親オブジェクト</param>
	void SetParent(FbxObject3d* parent) { parent_ = parent; }

private: // メンバ変数
	std::vector<std::unique_ptr<Object3d>> objParticle_;
	std::vector<ObjectParticle> particleData_;
	FbxObject3d* parent_ = nullptr;

};

