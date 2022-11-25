#pragma once

#include <DirectXMath.h>

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider : public BaseCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MeshCollider()
	{
		// ���b�V���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	/// <param name="model">���f��</param>
	void ConstructTriangles(Model* model);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <param name="reject"></param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

private:
	std::vector<Triangle> triangles_;
	// ���[���h�s��̋t�s��
	DirectX::XMMATRIX invMatWorld_ = DirectX::XMMatrixIdentity();
};