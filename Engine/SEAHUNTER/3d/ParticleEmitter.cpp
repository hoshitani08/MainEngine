#include "ParticleEmitter.h"
#include "BaseCalculate.h"

ParticleEmitter::ParticleEmitter(ParticleManager* particleMan)
{
	particleMan_ = particleMan;
}

ParticleEmitter::ParticleEmitter(ObjParticle* objParticle)
{
	objParticle_ = objParticle;
}

void ParticleEmitter::BubbleAdd(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.x;
		position_.y = position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.y = mdVel_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, position_, velocity_, accel_, objStartScale_, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::BloodAdd(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.x;
		position_.y = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.z = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel_.y = -RandCalculate(0.0f, 1.0f) * mdAcc_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			float size = RandCalculate(0.1f, objStartScale_.x);
			objParticle_->Add(life, position_, velocity_, accel_, { size, size, size }, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::ItemAdd(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * objMdPos_.x - objMdPos_.x / 2.0f) + position.x;
		position_.y = (RandCalculate(0.0f, 1.0f) * objMdPos_.y - objMdPos_.y / 2.0f) + position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * objMdPos_.z - objMdPos_.z / 2.0f) + position.z;
		//Y[+0.1]
		velocity_.y = mdVel_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, {}, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			XMFLOAT3 size = { objStartScale_.x,  RandCalculate(objStartScale_.y, 1.5f), objStartScale_.z };
			
			objParticle_->Add(life, position_, velocity_, accel_, size, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::HealAdd(int count, int life, XMFLOAT3 position, Model* model, float size)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * objMdPos_.x - objMdPos_.x / 2.0f) + position.x;
		position_.y = (RandCalculate(0.0f, 1.0f) * objMdPos_.y - objMdPos_.y / 2.0f) + position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * objMdPos_.z - objMdPos_.z / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.z = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, {}, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			float temp = RandCalculate(objStartScale_.x, size);
			XMFLOAT3 size = { temp,  temp, temp };

			objParticle_->Add(life, position_, velocity_, accel_, size, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::SandAdd(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.x;
		position_.y = position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = RandCalculate(0.0f, 0.02f);
		velocity_.z = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, position_, velocity_, accel_, objStartScale_, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::Add(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		position_.x = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.x;
		position_.y = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.y;
		position_.z = (RandCalculate(0.0f, 1.0f) * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		velocity_.z = RandCalculate(0.0f, 1.0f) * mdVel_ - mdVel_ / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel_.y = -RandCalculate(0.0f, 1.0f) * mdAcc_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, position_, velocity_, accel_, objStartScale_, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::Update()
{
	if (particleMan_ != nullptr)
	{
		particleMan_->Update();
	}

	if (objParticle_ != nullptr)
	{
		objParticle_->Update();
	}
}

void ParticleEmitter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (particleMan_ != nullptr)
	{
		particleMan_->Draw(cmdList);
	}

	if (objParticle_ != nullptr)
	{
		objParticle_->Draw(cmdList);
	}
}