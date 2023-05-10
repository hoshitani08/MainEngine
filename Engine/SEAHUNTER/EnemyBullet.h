#pragma once

#include "Object3d.h"

class EnemyBullet
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBullet() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet() = default;
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="rotation">回転</param>
	/// <param name="vector">スピード</param>
	/// <returns></returns>
	static std::unique_ptr<EnemyBullet> Create(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="rotation">回転</param>
	/// <param name="vector">スピード</param>
	void Initialize(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector);
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
	/// 死んだか
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool IsEndFlag() { return endFlag_; }

	bool Hit(const XMFLOAT3 position);

private: // メンバ変数
	// 弾
	std::unique_ptr<Object3d> bullet_;
	// 進行方向
	XMFLOAT3 speedVector_ = {};
	// 死亡フラグ
	bool endFlag_ = false;
	// 生きてる時間
	int lifeTimer_ = 0;
	// 当たったか
	bool hitFlag_ = false;
};

