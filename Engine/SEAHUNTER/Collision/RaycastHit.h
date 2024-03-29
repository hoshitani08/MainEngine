#pragma once

#include <DirectXMath.h>

#include "BaseCollider.h"

class Object3d;

struct RaycastHit
{
	// 衝突相手のオブジェクト
	Object3d* object_ = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider_ = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter_;
	// 衝突点までの距離
	float distance_ = 0.0f;
};