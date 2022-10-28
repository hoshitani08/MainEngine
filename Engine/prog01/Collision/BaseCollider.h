#pragma once

#include "CollisionInfo.h"
#include "Object3d.h"

//衝突判定用関連の型定義
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定

	COLLISIONSHAPE_SPHERE, // 球
	COLLISIONSHAPE_MESH, // メッシュ
};

class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d * object)
	{
		object3d_ = object;
	}

	inline Object3d* GetObject3d()
	{
		return object3d_;
	}

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d_->OnCollision(info);
	}

	// 当たり判定属性をセット
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}

	// 当たり判定属性を追加
	inline void AddAttribute(unsigned short attribute)
	{
		attribute_ |= attribute;
	}

	// 当たり判定属性を削除
	inline void RemoveAttribute(unsigned short attribute)
	{
		attribute_ &= !attribute;
	}

protected:
	Object3d* object3d_ = nullptr;
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute_ = 0b1111111111111111;
};