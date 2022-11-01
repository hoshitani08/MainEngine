#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

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

	struct CoralData
	{
		//珊瑚
		std::unique_ptr<Object3d> coralBlock;
		// 泡のエミッター
		std::unique_ptr<ParticleEmitter> bubbleEmitter;
		// 泡のパーティクル
		std::unique_ptr<ObjParticle> bubbleParticle;
		// 泡のタイマー
		float bubbleTimer = 0.0f;
		// 泡が出て来る時間
		int bubbleTimerMax = 0;
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
	// ランダムの計算
	const float RandCalculate(float a, float b);

private: // メンバ変数
	// 岩
	std::unique_ptr<Object3d> rockBlock_;
	// 珊瑚のデータ
	std::vector<CoralData> coralBlock_;
	// ブロックのタイプ
	BlockType blockType_;
};

