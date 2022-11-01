#pragma once

#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"
#include "ObjParticle.h"
#include "ParticleEmitter.h"

class Block
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �T�u�N���X
	enum class BlockType
	{
		Rock,
		Coral,
	};

public: // �����o�֐�
	// �R���X�g���N�^
	Block(int type, float posX, float posZ);
	// ������
	void Initialize(int type, float posX, float posZ);
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return block_->GetPosition(); }

private: // �����o�ϐ�
	// 
	std::unique_ptr<Object3d> block_;
	// 
	std::unique_ptr<ParticleEmitter> bubbleEmitter_;
	//
	std::unique_ptr<ObjParticle> bubbleParticle_;
	//
	float bubbleTimer_ = 0.0f;
	//
	int bubbleTimerMax_ = 0;
	//
	BlockType blockType_;
};

