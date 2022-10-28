#pragma once

#include "CollisionInfo.h"
#include "Object3d.h"

//�Փ˔���p�֘A�̌^��`
enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // ���ݒ�

	COLLISIONSHAPE_SPHERE, // ��
	COLLISIONSHAPE_MESH, // ���b�V��
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

	//�X�V
	virtual void Update() = 0;
	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	//�Փˎ��R�[���o�b�N�֐�
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d_->OnCollision(info);
	}

	// �����蔻�葮�����Z�b�g
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}

	// �����蔻�葮����ǉ�
	inline void AddAttribute(unsigned short attribute)
	{
		attribute_ |= attribute;
	}

	// �����蔻�葮�����폜
	inline void RemoveAttribute(unsigned short attribute)
	{
		attribute_ &= !attribute;
	}

protected:
	Object3d* object3d_ = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
	// �����蔻�葮��
	unsigned short attribute_ = 0b1111111111111111;
};