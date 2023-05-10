#pragma once

#include "Object3d.h"

class EnemyBullet
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyBullet() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyBullet() = default;
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="position">�ʒu</param>
	/// <param name="rotation">��]</param>
	/// <param name="vector">�X�s�[�h</param>
	/// <returns></returns>
	static std::unique_ptr<EnemyBullet> Create(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">�ʒu</param>
	/// <param name="rotation">��]</param>
	/// <param name="vector">�X�s�[�h</param>
	void Initialize(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 vector);
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
	/// ���񂾂�
	/// </summary>
	/// <returns>���S�t���O</returns>
	bool IsEndFlag() { return endFlag_; }

	bool Hit(const XMFLOAT3 position);

private: // �����o�ϐ�
	// �e
	std::unique_ptr<Object3d> bullet_;
	// �i�s����
	XMFLOAT3 speedVector_ = {};
	// ���S�t���O
	bool endFlag_ = false;
	// �����Ă鎞��
	int lifeTimer_ = 0;
	// ����������
	bool hitFlag_ = false;
};

