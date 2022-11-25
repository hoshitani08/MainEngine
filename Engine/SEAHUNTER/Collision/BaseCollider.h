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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCollider() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCollider() = default;
	/// <summary>
	/// オブジェクトを設定
	/// </summary>
	/// <param name="object">オブジェクト(OBJ)</param>
	inline void SetObject(Object3d * object)
	{
		object3d_ = object;
	}
	/// <summary>
	/// オブジェクトの取得
	/// </summary>
	/// <returns>オブジェクト</returns>
	inline Object3d* GetObject3d()
	{
		return object3d_;
	}
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 形状タイプ取得
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d_->OnCollision(info);
	}
	/// <summary>
	/// 当たり判定属性をセット
	/// </summary>
	/// <param name="attribute"></param>
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}
	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute"></param>
	inline void AddAttribute(unsigned short attribute)
	{
		attribute_ |= attribute;
	}
	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute"></param>
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