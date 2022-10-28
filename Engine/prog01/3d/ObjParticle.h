#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include <vector>

#include "Object3d.h"

class ObjParticle
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

	//�p�[�e�B�N��1��
	class ObjectParticle
	{
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//���x
		XMFLOAT3 velocity = {};
		//�����x
		XMFLOAT3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int numFrame = 0;
		// �����l
		XMFLOAT3 startScale = { 1.0f, 1.0f, 1.0f };
		// �ŏI�l
		XMFLOAT3 endScale = { 0.0f, 0.0f, 0.0f };
		// �F(RGBA)�����l
		XMFLOAT4 startColor = {};
		// �F(RGBA)�ŏI�l
		XMFLOAT4 endColor = {};
	};

public: // �����o�֐�
	ObjParticle();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�p�[�e�B�N���̒ǉ�
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		XMFLOAT3 startScale, XMFLOAT3 endScale, XMFLOAT4 startColor, XMFLOAT4 endColor, Model* model);

private: // �����o�ϐ�
	std::vector<std::unique_ptr<Object3d>> objParticle_;
	std::vector<ObjectParticle> particleData_;
};

