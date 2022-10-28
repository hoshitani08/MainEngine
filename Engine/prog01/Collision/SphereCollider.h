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
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// ���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	// �X�V
	void Update() override;

	inline const XMVECTOR& GetOffset() { return offset_; }

	inline void SetOffset(const XMVECTOR& offset) { offset_ = offset; }

	inline float GetRadius() { return radius_; }

	inline void SetRadius(float radius) { radius_ = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset_;
	// ���a
	float radius_;
};