#pragma once

#include <memory>
#include <array>
#include <algorithm>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "Sprite.h"

class LoadScene : public BaseScene
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
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LoadScene();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
	/// <summary>
	/// �G�t�F�N�g����`��
	/// </summary>
	void EffectDraw() override;

private: // �����o�ϐ�
	// �w�i
	std::unique_ptr<Sprite> black_;
	// 
	std::unique_ptr<Sprite> sphere_;
	//
	std::unique_ptr<Sprite> loading_;
	//
	float angle = 0.0f;
	//
	int timer_ = 0;
};

