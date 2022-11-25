#pragma once

#include <DirectXMath.h>

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// ���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	/// <summary>
	/// �I�t�Z�b�g�̎擾
	/// </summary>
	/// <returns>�I�t�Z�b�g</returns>
	inline const XMVECTOR& GetOffset() { return offset_; }
	/// <summary>
	/// �I�t�Z�b�g�̐ݒ�
	/// </summary>
	/// <param name="offset">�I�t�Z�b�g</param>
	inline void SetOffset(const XMVECTOR& offset) { offset_ = offset; }
	/// <summary>
	/// ���a�̎擾
	/// </summary>
	/// <returns>���a</returns>
	inline float GetRadius() { return radius_; }
	/// <summary>
	/// ���a�̐ݒ�
	/// </summary>
	/// <param name="radius">���a</param>
	inline void SetRadius(float radius) { radius_ = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset_;
	// ���a
	float radius_;
};