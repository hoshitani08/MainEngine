#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CollisionPrimitive.h"
#include "Camera.h"
#include "LightGroup.h"
#include "FbxObject3d.h"
#include "Hunter.h"
#include "Monster.h"
#include "UserInterface.h"
#include "Block.h"
#include "Stage.h"
#include "SceneChange.h"
#include "Ease.h"
#include "GameCamera.h"

class CollisionManager;
class Player;
class ContactableObject;

class GameScene : public BaseScene
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
	struct Quest
	{
		int timer = 0;
		int second = 0;
		int minute = 0;
	};

private: // 静的定数
	static const int DEBUG_TEXT_TEX_NUMBER = 0;

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// エフェクトあり描画
	/// </summary>
	void EffectDraw() override;
	/// <summary>
	/// プレイヤーの攻撃判定
	/// </summary>
	void PlayerAttack();
	/// <summary>
	/// 開始
	/// </summary>
	void GameStrat();
	/// <summary>
	/// 終了
	/// </summary>
	void GameOverEnd();
	/// <summary>
	/// ゲーム中
	/// </summary>
	void GamePlay();

private: // メンバ変数
	//カメラ
	std::unique_ptr<GameCamera> camera_;
	//UI
	std::unique_ptr<UserInterface> ui_;
	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_;
	//オブジェクト
	std::unique_ptr<Hunter> hunter_;
	std::unique_ptr<Monster> monster_;
	std::unique_ptr<Object3d> hitSphere_;
	std::unique_ptr<Stage> stage_;
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//当たり判定
	CollisionManager* collisionManager_ = nullptr;
	// タイマー
	Quest quest_;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = 0;
};