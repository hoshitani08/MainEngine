#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct CollisionInfo // 衝突情報
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::XMVECTOR& inter)
	{
		object_ = object;
		collider_ = collider;
		inter_ = inter;
	}

	//衝突相手のオブジェクト
	Object3d* object_ = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider_ = nullptr;
	//衝突点
	DirectX::XMVECTOR inter_;
};