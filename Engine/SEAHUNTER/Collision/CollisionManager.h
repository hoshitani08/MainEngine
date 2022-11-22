#pragma once

#include <d3d12.h>
#include <forward_list>

#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

class BaseCollider;

class CollisionManager
{
public:// �ÓI�����o�֐�
	static CollisionManager* GetInstance();

public:// �����o�֐�
	// �R���C�_�[�̒ǉ�
	inline void AddCollider(BaseCollider* collider)
	{
		colliders_.push_front(collider);
	}

	// �R���C�_�[�̍폜
	inline void RemoveCollider(BaseCollider* collider)
	{
		colliders_.remove(collider);
	}

	// �S�Ă̏Փ˃`�F�b�N
	void CheckAllCollisions();
	// ���C�L���X�g
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	// ���ɂ��ՓˑS����
	void QuerySphere(const Sphere& sphere, QueryCallback* callback,
		unsigned short attribute = (unsigned short)0xffffffff);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// �R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders_;
};