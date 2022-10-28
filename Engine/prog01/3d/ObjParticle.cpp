#include "ObjParticle.h"
#include "ObjFactory.h"
#include "BaseCalculate.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ObjParticle::ObjParticle()
{
}

void ObjParticle::Finalize()
{
}

void ObjParticle::Update()
{
	for (int i = 0; i < particleData_.size(); i++)
	{
		if (particleData_[i].frame >= particleData_[i].num_frame)
		{
			particleData_.erase(particleData_.begin() + i);
			objParticle_.erase(objParticle_.begin() + i);
		}
	}

	//全パーティクル更新
	for (int i = 0; i < particleData_.size(); i++)
	{
		//経過フレーム数をカウント
		particleData_[i].frame++;
		// 進行度を0～1の範囲に換算
		float f = (float)particleData_[i].num_frame / particleData_[i].frame;
		//速度に加速度を加算
		particleData_[i].velocity = particleData_[i].velocity + particleData_[i].accel;
		//速度による移動
		objParticle_[i]->SetPosition(objParticle_[i]->GetPosition() + particleData_[i].velocity);
		//スケールの線形補間
		objParticle_[i]->SetScale(particleData_[i].s_scale + (particleData_[i].e_scale - particleData_[i].s_scale) / f);
		// カラーの線形補間
		objParticle_[i]->SetColor(particleData_[i].s_color + (particleData_[i].e_color - particleData_[i].s_color) / f);
	}

	for (auto& a : objParticle_)
	{
		a->Update();
	}
}

void ObjParticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	Object3d::PreDraw(cmdList);
	for (auto& a : objParticle_)
	{
		a->Draw();
	}
	Object3d::PostDraw();
}

void ObjParticle::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, XMFLOAT3 start_scale, XMFLOAT3 end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color, Model* model)
{
	ObjectParticle p;
	//値のセット
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.s_color = start_color;
	p.e_color = end_color;

	std::unique_ptr<Object3d> a = Object3d::Create(model);
	a->SetBillboard(true);

	a->SetPosition(position);

	objParticle_.push_back(std::move(a));

	particleData_.push_back(p);
}
