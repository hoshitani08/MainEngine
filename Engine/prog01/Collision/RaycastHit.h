#pragma once

#include <DirectXMath.h>

#include "BaseCollider.h"

class Object3d;

struct RaycastHit
{
	// �Փˑ���̃I�u�W�F�N�g
	Object3d* object_ = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter_;
	// �Փ˓_�܂ł̋���
	float distance_ = 0.0f;
};