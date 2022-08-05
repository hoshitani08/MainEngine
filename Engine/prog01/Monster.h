#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Hunter.h"
#include "FbxObject3d.h"

enum class Phase
{
	Approach,
	Stop,
	Attack,
	Leave
};

enum class AttackType
{
	Weak,
	Ordinary,
	Strong
};

class Monster
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

public: // �ÓI�����o�֐�
	static std::unique_ptr<Monster> Create();

public: // �����o�֐�
	// ������
	void Initialize();
	//�I��
	void Finalize();
	// ���t���[������
	void Update();
	// �`��
	void Draw();

	void Move();

	void SetHunter(Hunter* hunter) { hunter_ = hunter; }

private: // �����o�ϐ�
	std::unique_ptr<FbxObject3d> monster_;
	Hunter* hunter_ = nullptr;

	float speed_ = 0.5f;
	bool avoidFlag_ = false;
	int moveTimer_ = 0;
	int max = 0;
	int count = 0;

	Phase phase_ = Phase::Approach;

	AttackType attackType_ = AttackType::Ordinary;
};