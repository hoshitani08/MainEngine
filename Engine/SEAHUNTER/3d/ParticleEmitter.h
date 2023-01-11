#pragma once

#include <memory>

#include "ParticleManager.h"
#include "ObjParticle.h"

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

	/// <summary>
	/// 追加(泡)
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void BubbleAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// 追加(血用)
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void BloodAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// 追加(アイテム用)
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void ItemAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// 追加(回復用)
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void HealAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr, float size = 0.5f);
	/// <summary>
	/// 追加(砂埃)
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void SandAdd(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="count">数</param>
	/// <param name="life">生存時間</param>
	/// <param name="position">座標</param>
	/// <param name="model">モデル</param>
	void Add(int count, int life, XMFLOAT3 position = { 0,0,0 }, Model* model = nullptr);
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
	/// 座標の設定
	/// </summary>
	/// <param name="mdPos">座標</param>
	void SetCenter(const float mdPos) { mdPos_ = mdPos; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="mdPos">座標</param>
	void SetCenter(const XMFLOAT3 objMdPos) { objMdPos_ = objMdPos; }
	/// <summary>
	/// 速度の設定
	/// </summary>
	/// <param name="mdVel">速度</param>
	void SetVelocity(float mdVel) { mdVel_ = mdVel; }
	/// <summary>
	/// 加速度の設定
	/// </summary>
	/// <param name="mdAcc">加速度</param>
	void SetAccel(float mdAcc) { mdAcc_ = mdAcc; }
	/// <summary>
	/// スケールの初期値の設定
	/// </summary>
	/// <param name="startScale">スケールの初期値</param>
	void SetStartScale(float startScale) { startScale_ = startScale; }
	/// <summary>
	/// スケールの初期値の設定
	/// </summary>
	/// <param name="startScale">スケールの初期値</param>
	void SetObjStartScale(XMFLOAT3 objStartScale) { objStartScale_ = objStartScale; }
	/// <summary>
	/// スケールの初期値の設定
	/// </summary>
	/// <param name="endScale">スケールの初期値</param>
	void SetEndScale(float endScale) { endScale_ = endScale; }
	/// <summary>
	/// スケールの初期値の設定
	/// </summary>
	/// <param name="endScale">スケールの初期値</param>
	void SetObjEndScale(XMFLOAT3 objEndScale) { objEndScale_ = objEndScale; }
	/// <summary>
	/// 色(RGBA)初期値の設定
	/// </summary>
	/// <param name="s_color">色(RGBA)初期値</param>
	void SetStartColor(XMFLOAT4 s_color) { startColor_ = s_color; }
	/// <summary>
	/// 色(RGBA)最終値の設定
	/// </summary>
	/// <param name="e_color">色(RGBA)最終値</param>
	void SetEndColor(XMFLOAT4 e_color) { endColor_ = e_color; }
	/// <summary>
	/// マネージャーの設定
	/// </summary>
	/// <param name="particleMan">マネージャー</param>
	void SetParticleManager(ParticleManager* particleMan) { particleMan_ = particleMan; }
	/// <summary>
	/// マネージャーの設定
	/// </summary>
	/// <param name="particleMan">マネージャー</param>
	void SetObjParticle(ObjParticle* objParticle) { objParticle_ = objParticle; }

private: // メンバ変数
	//座標
	XMFLOAT3 position_ = {};
	//速度
	XMFLOAT3 velocity_ = {};
	//加速度
	XMFLOAT3 accel_ = {};
	// スケール初期値
	float startScale_ = 1.0f;
	XMFLOAT3 objStartScale_ = { 1.0f, 1.0f, 1.0f };
	// スケール最終値
	float endScale_ = 0.0f;
	XMFLOAT3 objEndScale_ = { 0.0f, 0.0f, 0.0f };
	// 色(RGBA)初期値
	XMFLOAT4 startColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 色(RGBA)最終値
	XMFLOAT4 endColor_ = {};
	//X,Y,Z全て[-5.0,+5.0]でランダムに分布
	float mdPos_ = 10.0f;
	XMFLOAT3 objMdPos_ = { 10.0f, 10.0f, 10.0f };
	//X,Y,Z全て[-0.05,+0.05]でランダムに分布
	float mdVel_ = 0.1f;
	//重力に見立ててYのみ[-0.001f,0]でランダムに分布
	float mdAcc_ = 0.001f;

	ParticleManager* particleMan_ = nullptr;
	ObjParticle* objParticle_ = nullptr;
};