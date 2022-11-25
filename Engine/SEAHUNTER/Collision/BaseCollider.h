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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseCollider() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseCollider() = default;
	/// <summary>
	/// �I�u�W�F�N�g��ݒ�
	/// </summary>
	/// <param name="object">�I�u�W�F�N�g(OBJ)</param>
	inline void SetObject(Object3d * object)
	{
		object3d_ = object;
	}
	/// <summary>
	/// �I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g</returns>
	inline Object3d* GetObject3d()
	{
		return object3d_;
	}
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`��^�C�v�擾
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }
	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d_->OnCollision(info);
	}
	/// <summary>
	/// �����蔻�葮�����Z�b�g
	/// </summary>
	/// <param name="attribute"></param>
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}
	/// <summary>
	/// �����蔻�葮����ǉ�
	/// </summary>
	/// <param name="attribute"></param>
	inline void AddAttribute(unsigned short attribute)
	{
		attribute_ |= attribute;
	}
	/// <summary>
	/// �����蔻�葮�����폜
	/// </summary>
	/// <param name="attribute"></param>
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