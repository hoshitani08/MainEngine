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
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// 球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	// 更新
	void Update() override;

	inline const XMVECTOR& GetOffset() { return offset_; }

	inline void SetOffset(const XMVECTOR& offset) { offset_ = offset; }

	inline float GetRadius() { return radius_; }

	inline void SetRadius(float radius) { radius_ = radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset_;
	// 半径
	float radius_;
};