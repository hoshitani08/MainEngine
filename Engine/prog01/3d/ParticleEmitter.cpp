#include "ParticleEmitter.h"

std::unique_ptr<ParticleEmitter> ParticleEmitter::Create(ParticleManager* particleMan)
{
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	if (particleEmitter == nullptr)
	{
		return nullptr;
	}

	particleEmitter->SetParticleManager(particleMan);

	return std::unique_ptr<ParticleEmitter>(particleEmitter);
}

void ParticleEmitter::BubbleAdd(XMFLOAT3 position)
{
	for (int i = 0; i < 1; i++)
	{
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		this->position_.x = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.x;
		this->position_.y = position.y;
		this->position_.z = ((float)rand() / RAND_MAX * md_pos_ - md_pos_ / 2.0f) + position.z;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		velocity_.y = md_vel_;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z

		//�ǉ�
		particleMan_->Add(900, this->position_, velocity_, accel_, s_scale_, e_scale_, s_color_, e_color_);
	}
}

void ParticleEmitter::Add(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++)
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
		particleMan_->Add(60, this->position_, velocity_, accel_, s_scale_, e_scale_, s_color_, e_color_);
	}
}

void ParticleEmitter::Update()
{
	particleMan_->Update();
}

void ParticleEmitter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particleMan_->Draw(cmdList);
}