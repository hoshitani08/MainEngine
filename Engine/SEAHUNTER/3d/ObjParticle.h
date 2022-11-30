#pragma once

#include <DirectXMath.h>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include <vector>

#include "FbxObject3d.h"
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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ObjParticle();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">���W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="startScale">�����l�̔{��</param>
	/// <param name="endScale">�ŏI�l�̔{��</param>
	/// <param name="startColor">�F(RGBA)�����l</param>
	/// <param name="endColor">�F(RGBA)�ŏI�l</param>
	/// <param name="model">���f��</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
		XMFLOAT3 startScale, XMFLOAT3 endScale, XMFLOAT4 startColor, XMFLOAT4 endColor, Model* model);
	/// <summary>
	/// �e�I�u�W�F�N�g�̐ݒ�
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	void SetParent(FbxObject3d* parent) { parent_ = parent; }

private: // �����o�ϐ�
	std::vector<std::unique_ptr<Object3d>> objParticle_;
	std::vector<ObjectParticle> particleData_;
	FbxObject3d* parent_ = nullptr;

};

