#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX& matWorld = object3d_->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center = matWorld.r[3] + offset_;
	Sphere::radius = radius_;
}