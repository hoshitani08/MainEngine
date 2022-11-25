#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <memory>

#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include "FbxObject3d.h"
#include "ShaderManager.h"

class Object3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;  //�r���[�v���W�F�N�V�����s��
		XMMATRIX world;     //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W(���[���h���W)
		float pad1;         //�p�f�B���O
		XMFLOAT4 color;     // �F
	};

private: // �萔

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="camera">�J����</param>
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	/// <summary>
	/// �ÓI�j��
	/// </summary>
	static void StaticFinalize();
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�������ꂽ�I�u�W�F�N�g</returns>
	static std::unique_ptr<Object3d> Create(Model* model = nullptr);
	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera) { Object3d::camera_ = camera; }
	/// <summary>
	/// ���C�g�̐ݒ�
	/// </summary>
	/// <param name="light">���C�g</param>
	static void SetLight(LightGroup* light) { Object3d::light_ = light; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �J����
	static Camera* camera_;
	//���C�g
	static LightGroup* light_;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3d() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object3d();
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �s��̍X�V
	/// </summary>
	void UpdateWorldMatrix();
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position_; }
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	/// <summary>
	/// X,Y,Z�����̎擾
	/// </summary>
	/// <returns>X,Y,Z�����̉�]</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }
	/// <summary>
	/// X,Y,Z�����̐ݒ�
	/// </summary>
	/// <param name="rotation">X,Y,Z�����̉�]</param>
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	/// <summary>
	/// �X�P�[���̎擾
	/// </summary>
	/// <returns>�X�P�[��</returns>
	const XMFLOAT3& GetScale() { return scale_; }
	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { model_ = model; };
	/// <summary>
	/// �r���{�[�h�̐ݒ�
	/// </summary>
	/// <param name="isBillboard">�r���{�[�h�����邩</param>
	void SetBillboard(bool isBillboard) { isBillboard_ = isBillboard; }
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	/// <summary>
	/// �R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="collider">�Փ˔���</param>
	void SetCollider(BaseCollider* collider);
	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}
	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	/// <returns>���[���h���W</returns>
	XMFLOAT3 GetWorldPosition();
	/// <summary>
	/// ���f�����擾
	/// </summary>
	/// <returns>���f��</returns>
	inline Model* GetModel() { return model_; }
	/// <summary>
	/// �F�̐ݒ�
	/// </summary>
	/// <param name="_color">�F</param>
	void SetColor(XMFLOAT4 _color) { color_ = _color; }
	/// <summary>
	/// �e�I�u�W�F�N�g�̐ݒ�
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	void SetParent(Object3d* parent) { parent_ = parent; }
	void SetParent(FbxObject3d* parent) { fbxParent_ = parent; }
	/// <summary>
	/// �擾����{�[���̖��O�̐ݒ�
	/// </summary>
	/// <param name="boneName">�{�[����</param>
	void SetBoneName(std::string boneName) { boneName_ = boneName; }
	/// <summary>
	/// �v���~�e�B�u�`��̐ݒ�
	/// </summary>
	/// <param name="type"></param>
	void SetPrimitiveType(ShaderManager::Type type) { type_ = type; }

protected: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0_; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color_ = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position_ = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld_ = {};
	// �e�I�u�W�F�N�g
	Object3d* parent_ = nullptr;
	FbxObject3d* fbxParent_ = nullptr;
	// ���f��
	Model* model_ = nullptr;
	// �r���{�[�h
	bool isBillboard_ = false;
	//�N���X��(�f�o�b�N�p)
	const char* name_ = nullptr;
	// �R���C�_�[
	BaseCollider* collider_ = nullptr;
	// �{�[���̃t�@�C����
	std::string boneName_;
	//
	ShaderManager::Type type_ = ShaderManager::Type::Triangle;
};