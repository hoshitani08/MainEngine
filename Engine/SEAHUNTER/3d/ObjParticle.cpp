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
		if (particleData_[i].frame >= particleData_[i].numFrame)
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
		float f = (float)particleData_[i].numFrame / particleData_[i].frame;
		//速度に加速度を加算
		particleData_[i].velocity = particleData_[i].velocity + particleData_[i].accel;
		//速度による移動
		if (parent_ != nullptr)
		{
			particleData_[i].position = particleData_[i].position + particleData_[i].velocity;
			objParticle_[i]->SetPosition(particleData_[i].position + parent_->GetWorldPosition());
		}
		else
		{
			particleData_[i].position = particleData_[i].position + particleData_[i].velocity;
			objParticle_[i]->SetPosition(particleData_[i].position);
		}
		//スケールの線形補間
		objParticle_[i]->SetScale(particleData_[i].startScale + (particleData_[i].endScale - particleData_[i].startScale) / f);
		// カラーの線形補間
		objParticle_[i]->SetColor(particleData_[i].startColor + (particleData_[i].endColor - particleData_[i].startColor) / f);
	}

	for (auto& a : objParticle_)
	{
		a->Update();
	}
}

void ObjParticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (auto& a : objParticle_)
	{
		a->Draw(cmdList);
	}
}

void ObjParticle::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, XMFLOAT3 startScale, XMFLOAT3 endScale, XMFLOAT4 startColor, XMFLOAT4 endColor, Model* model)
{
	ObjectParticle p;
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.numFrame = life;
	p.startScale = startScale;
	p.endScale = endScale;
	p.startColor = startColor;
	p.endColor = endColor;

	std::unique_ptr<Object3d> a = Object3d::Create(model);
	a->SetBillboard(true);
	objParticle_.push_back(std::move(a));

	particleData_.push_back(p);
}
