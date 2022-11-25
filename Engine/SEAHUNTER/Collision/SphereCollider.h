#pragma once

#include <DirectXMath.h>

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// 球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// オフセットの取得
	/// </summary>
	/// <returns>オフセット</returns>
	inline const XMVECTOR& GetOffset() { return offset_; }
	/// <summary>
	/// オフセットの設定
	/// </summary>
	/// <param name="offset">オフセット</param>
	inline void SetOffset(const XMVECTOR& offset) { offset_ = offset; }
	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns>半径</returns>
	inline float GetRadius() { return radius_; }
	/// <summary>
	/// 半径の設定
	/// </summary>
	/// <param name="radius">半径</param>
	inline void SetRadius(float radius) { radius_ = radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset_;
	// 半径
	float radius_;
};