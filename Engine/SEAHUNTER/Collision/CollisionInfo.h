#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct CollisionInfo // �Փˏ��
{
public:
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::XMVECTOR& inter)
	{
		object_ = object;
		collider_ = collider;
		inter_ = inter;
	}

	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object_ = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter_;
};