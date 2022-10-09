#include "ParticleEmitter.h"

std::unique_ptr<ParticleEmitter> ParticleEmitter::Create()
{
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	if (particleEmitter == nullptr)
	{
		return nullptr;
	}

	return std::unique_ptr<ParticleEmitter>(particleEmitter);
}

void ParticleEmitter::BubbleAdd(XMFLOAT3 position)
{
	for (int i = 0; i < 1; i++)
	{
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
		this->position.y = position.y;
		this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		velocity.y = md_vel;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z

		//�ǉ�
		particleMan->Add(900, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::Add(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z�S��[-5.0,+5.0]�Ń����_���ɕ��z
		this->position.x = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.x;
		this->position.y = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.y;
		this->position.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) + position.z;
		//X,Y,Z�S��[-0.05,+0.05]�Ń����_���ɕ��z
		velocity.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		velocity.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		velocity.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		accel.y = -(float)rand() / RAND_MAX * md_acc;

		//�ǉ�
		particleMan->Add(60, this->position, velocity, accel, s_scale, e_scale, s_color, e_color);
	}
}

void ParticleEmitter::Update()
{
	particleMan->Update();
}

void ParticleEmitter::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particleMan->Draw(cmdList);
}