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
		this->position_.x = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.x;
		this->position_.y = position.y;
		this->position_.z = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.y = mdVel_;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, this->position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, this->position_, velocity_, accel_, objStartScale_, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::BloodAdd(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position_.x = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.x;
		this->position_.y = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.y;
		this->position_.z = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		velocity_.z = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel_.y = -(float)rand() / RAND_MAX * mdAcc_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, this->position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			float size = RandCalculate(0.1f, objStartScale_.x);
			objParticle_->Add(life, this->position_, velocity_, accel_, { size, size, size }, objEndScale_, startColor_, endColor_, model);
		}
	}
}

void ParticleEmitter::Add(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z全て[-5.0,+5.0]でランダムに分布
		this->position_.x = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.x;
		this->position_.y = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.y;
		this->position_.z = ((float)rand() / RAND_MAX * mdPos_ - mdPos_ / 2.0f) + position.z;
		//X,Y,Z全て[-0.05,+0.05]でランダムに分布
		velocity_.x = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		velocity_.y = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		velocity_.z = (float)rand() / RAND_MAX * mdVel_ - mdVel_ / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		accel_.y = -(float)rand() / RAND_MAX * mdAcc_;

		//追加
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, this->position_, velocity_, accel_, startScale_, endScale_, startColor_, endColor_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, this->position_, velocity_, accel_, objStartScale_, objEndScale_, startColor_, endColor_, model);
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