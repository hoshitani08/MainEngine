#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

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

	struct CoralData
	{
		//�X��
		std::unique_ptr<Object3d> coralBlock;
		// �A�̃G�~�b�^�[
		std::unique_ptr<ParticleEmitter> bubbleEmitter;
		// �A�̃p�[�e�B�N��
		std::unique_ptr<ObjParticle> bubbleParticle;
		// �A�̃^�C�}�[
		float bubbleTimer = 0.0f;
		// �A���o�ė��鎞��
		int bubbleTimerMax = 0;
	};

public: // �����o�֐�
	// �R���X�g���N�^
	Block(int type, XMFLOAT3 pos);
	// ������
	void Initialize(int type, XMFLOAT3 pos);
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	// ��
	std::unique_ptr<Object3d> rockBlock_;
	// �X��̃f�[�^
	std::vector<CoralData> coralBlock_;
	// �u���b�N�̃^�C�v
	BlockType blockType_;
};

