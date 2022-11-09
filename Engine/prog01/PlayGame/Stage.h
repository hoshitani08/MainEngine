#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "Object3d.h"
#include "Block.h"

class Stage
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

public: // �����o�֐�
	// �R���X�g���N�^
	Stage();
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	//
	std::unique_ptr<Object3d> skydome_;
	//
	std::unique_ptr<Object3d> ground_;
	//
	std::vector<std::unique_ptr<Block>> block_;
	//
	std::unique_ptr<Object3d> field_;
};

