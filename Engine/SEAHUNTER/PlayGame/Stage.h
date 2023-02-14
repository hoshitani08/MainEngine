#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "Object3d.h"
#include "Block.h"
#include "Hunter.h"
#include "Monster.h"
#include "ObjParticle.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"

class Stage
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

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage(Monster* monster, Hunter* hunter, Camera* camera);
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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

private: // メンバ変数
	// 水面
	std::unique_ptr<Object3d> watersurface_;
	// 地面
	std::unique_ptr<Object3d> ground_;
	// 岩や珊瑚のオブジェクト
	std::vector<std::unique_ptr<Block>> block_;
	// 行動範囲
	std::unique_ptr<Object3d> field_;
	// モンスターのポインタ
	Monster* monster_ = nullptr;
	// ハンターのポインタ
	Hunter* hunter_ = nullptr;
	//カメラ
	Camera* camera_ = nullptr;
	// 砂埃のエミッター
	std::unique_ptr<ParticleEmitter> fugitiveBustEmitter;
	// 砂埃のパーティクル
	std::unique_ptr<ParticleManager> fugitiveBustParticle;
};

