#include "EnemyBullet.h"
#include "ObjFactory.h"
#include "BaseCalculate.h"
#include "Collision.h"

std::unique_ptr<EnemyBullet> EnemyBullet::Create(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector)
{
	EnemyBullet* temp = new EnemyBullet();

	if (temp == nullptr)
	{
		return nullptr;
	}
	temp->Initialize(position, rotation, vector);

	return std::unique_ptr<EnemyBullet>(temp);
}

void EnemyBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector)
{
	bullet_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
	bullet_->SetPosition(position);
	bullet_->SetRotation(rotation);

	speedVector_ = vector;
}

void EnemyBullet::Update()
{
	XMFLOAT3 pos = bullet_->GetPosition();

	pos = pos + speedVector_;

	bullet_->SetPosition(pos);

	if (lifeTimer_ >= 300 && !endFlag_ || hitFlag_)
	{
		XMFLOAT3 scale = bullet_->GetScale();

		scale.x -= 0.1f;
		scale.y -= 0.1f;
		scale.z -= 0.1f;

		bullet_->SetScale(scale);
	}

	if (bullet_->GetScale().x <= 0.0f)
	{
		endFlag_ = true;
	}

	lifeTimer_++;

	bullet_->Update();
}

void EnemyBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bullet_->Draw(cmdList);
}

bool EnemyBullet::Hit(const XMFLOAT3 position)
{
	if (hitFlag_)
	{
		return false;
	}

	Sphere player, bullet;

	player.center = { position.x, position.y, position.z };
	bullet.center = { bullet_->GetPosition().x, bullet_->GetPosition().y, bullet_->GetPosition().z };

	if (Collision::CheckSphere2Sphere(player, bullet))
	{
		hitFlag_ = true;
		return true;
	}

	return false;
}
