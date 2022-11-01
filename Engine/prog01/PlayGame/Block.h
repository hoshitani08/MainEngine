#pragma once

#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"
#include "ObjParticle.h"
#include "ParticleEmitter.h"

class Block
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

public: // サブクラス
	enum class BlockType
	{
		Rock,
		Coral,
	};

public: // メンバ関数
	// コンストラクタ
	Block(int type, float posX, float posZ);
	// 初期化
	void Initialize(int type, float posX, float posZ);
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// 座標の取得
	const XMFLOAT3& GetPosition() { return block_->GetPosition(); }

private: // メンバ変数
	// 
	std::unique_ptr<Object3d> block_;
	// 
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
	//
	std::unique_ptr<ObjParticle> bubbleParticle_;
	//
	float bubbleTimer_ = 0.0f;
	//
	int bubbleTimerMax_ = 0;
	//
	BlockType blockType_;
};

