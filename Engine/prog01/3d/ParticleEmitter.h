#pragma once

#include "ParticleManager.h"
#include "ObjParticle.h"

#include <memory>

class ParticleEmitter
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	//

public: // メンバ関数
	ParticleEmitter(ParticleManager* particleMan);
	ParticleEmitter(ObjParticle* objParticle);

	// 追加
	void BubbleAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	void Add(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// 座標の設定
	void SetCenter(const float md_pos) { md_pos_ = md_pos; }
	// 速度の設定
	void SetVelocity(float md_vel) { md_vel_ = md_vel; }
	// 加速度の設定
	void SetAccel(float md_acc) { md_acc_ = md_acc; }
	// スケールの設定
	void SetObjScale(XMFLOAT3 objScale) { os_scale = objScale; }

	//色(RGBA)初期値の設定
	void SetStartColor(XMFLOAT4 s_color) { s_color_ = s_color; }
	//色(RGBA)最終値の設定
	void SetEndColor(XMFLOAT4 e_color) { e_color_ = e_color; }
	// マネージャーの設定
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	void SetObjParticle(ObjParticle* objParticle) { objParticle_ = objParticle; }

private: // メンバ変数
	//座標
	XMFLOAT3 position_ = {};
	//速度
	XMFLOAT3 velocity_ = {};
	//加速度
	XMFLOAT3 accel_ = {};
	//現在フレーム
	int frame_ = 0;
	//終了フレーム
	int num_frame_ = 0;
	// スケール初期値
	float s_scale_ = 1.0f;
	XMFLOAT3 os_scale = { 1.0f, 1.0f, 1.0f };
	// スケール最終値
	float e_scale_ = 0.0f;
	XMFLOAT3 oe_scale = { 0.0f, 0.0f, 0.0f };
	// 色(RGBA)初期値
	XMFLOAT4 s_color_ = { 1, 1, 1, 1};
	// 色(RGBA)最終値
	XMFLOAT4 e_color_ = {};
	//X,Y,Z全て[-5.0,+5.0]でランダムに分布
	float md_pos_ = 10.0f;
	//X,Y,Z全て[-0.05,+0.05]でランダムに分布
	float md_vel_ = 0.1f;
	//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	float md_acc_ = 0.001f;

	ParticleManager* particleMan_ = nullptr;
	ObjParticle* objParticle_ = nullptr;
};