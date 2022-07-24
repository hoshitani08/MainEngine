#pragma once
#include <DirectXMath.h>
#include <memory>

#include "FbxObject3d.h"

class Hunter
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

public: // 静的メンバ関数
	static std::unique_ptr<Hunter> Create();

public: // メンバ関数
	// 初期化
	void Initialize();
	//終了
	void Finalize();
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw();

	void Move();
	void Attack();

	void SetAngle(XMFLOAT2 angle) { cameraAngle_ = angle; }

	void SetVector(XMVECTOR vector) { vector_ = vector; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position_; }

private: // メンバ変数
	//　モデル
	std::unique_ptr<FbxObject3d> hunter_;
	//　位置
	XMFLOAT3 position_;
	//　移動倍率
	float speed_ = 0.0f;
	//　回避フラグ
	bool avoidFlag_ = false;
	//　回避タイマー
	int avoidTimer_ = 0;
	//　カメラの角度
	XMFLOAT2 cameraAngle_ = {};

	XMVECTOR vector_;
};