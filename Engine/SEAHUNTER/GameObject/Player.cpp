#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	// コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	// A,Dで旋回
	if (input_->PushKey(DIK_A))
	{
		rotation_.y -= 2.0f;
	}
	else if (input_->PushKey(DIK_D))
	{
		rotation_.y += 2.0f;
	}

	// 移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	move = XMVector3TransformNormal(move, matRot);

	// 向いている方向に移動
	if (input_->PushKey(DIK_S))
	{
		position_.x -= move.m128_f32[0];
		position_.y -= move.m128_f32[1];
		position_.z -= move.m128_f32[2];
	}
	else if (input_->PushKey(DIK_W))
	{
		position_.x += move.m128_f32[0];
		position_.y += move.m128_f32[1];
		position_.z += move.m128_f32[2];
	}

	// 落下処理
	if (!onGround_)
	{
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV_.m128_f32[1] = max(fallV_.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		position_.x += fallV_.m128_f32[0];
		position_.y += fallV_.m128_f32[1];
		position_.z += fallV_.m128_f32[2];
	}
	// ジャンプ操作
	else if (input_->TriggerKey(DIK_SPACE))
	{
		onGround_ = false;
		const float jumpVYFist = 0.2f; //ジャンプ時上向き初速
		fallV_ = { 0, jumpVYFist, 0, 0 };
	}

	//コライダー更新
	UpdateWorldMatrix();
	collider_->Update();

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider_);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info)
		{
			//ワールドの上方向
			const XMVECTOR up = { 0,1,0,0 };
			//排斥方向
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//上方向と排斥方向の角度差のコサイン値
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(30.0f));
			//角度差によって天井または地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold)
			{
				//球を排斥(押し出す)
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		//クエリーに使用する球
		Sphere* sphere = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリーコールバックの関数オブジェクト
	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider,
		&callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position_.x += callback.move.m128_f32[0];
	position_.y += callback.move.m128_f32[1];
	position_.z += callback.move.m128_f32[2];
	//コライダー更新
	UpdateWorldMatrix();
	collider_->Update();

	// 球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround_)
	{
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround_ = true;
			position_.y -= (raycastHit.distance_ - sphereCollider->GetRadius() * 2.0f);
			// 行列の更新など
			Object3d::Update();
		}
		// 地面がないので落下
		else
		{
			onGround_ = false;
			fallV_ = {};
		}
	}
	// 落下状態
	else if (fallV_.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			// 着地
			onGround_ = true;
			position_.y -= (raycastHit.distance_ - sphereCollider->GetRadius() * 2.0f);
			// 行列の更新など
			Object3d::Update();
		}
	}

	// 行列の更新など
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//衝突点にパーティクルを発生する
	//for (int i = 0; i < 1; ++i)
	//{
	//
	//	const float rnd_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = static_cast<float>(rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f);
	//	vel.y = static_cast<float>(rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f);
	//	vel.z = static_cast<float>(rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f);
	//
	//	particleMan->Add(15, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f, { 1, 1, 1, 1 }, {});
	//}
}