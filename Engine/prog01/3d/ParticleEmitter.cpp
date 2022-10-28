#include "ParticleEmitter.h"

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
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		this->position_.x = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.x;
		this->position_.y = position.y;
		this->position_.z = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.z;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		velocity_.y = md_vel_;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z

		//�ǉ�
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, this->position_, velocity_, accel_, s_scale_, e_scale_, s_color_, e_color_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, this->position_, velocity_, accel_, os_scale, oe_scale, s_color_, e_color_, model);
		}
	}
}

void ParticleEmitter::Add(int count, int life, XMFLOAT3 position, Model* model)
{
	for (int i = 0; i < count; i++)
	{
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		this->position_.x = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.x;
		this->position_.y = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.y;
		this->position_.z = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.z;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		velocity_.x = (float)rand() / RAND_MAX * md_vel_ - md_vel_ / 2.0f;
		velocity_.y = (float)rand() / RAND_MAX * md_vel_ - md_vel_ / 2.0f;
		velocity_.z = (float)rand() / RAND_MAX * md_vel_ - md_vel_ / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		accel_.y = -(float)rand() / RAND_MAX * md_acc_;

		//�ǉ�
		if (particleMan_ != nullptr)
		{
			particleMan_->Add(life, this->position_, velocity_, accel_, s_scale_, e_scale_, s_color_, e_color_);
		}

		if (objParticle_ != nullptr && model != nullptr)
		{
			objParticle_->Add(life, this->position_, velocity_, accel_, os_scale, oe_scale, s_color_, e_color_, model);
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