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
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// �ÓI�j��
	static void StaticFinalize();
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<Object3d> Create(Model* model = nullptr);
	// �J�����̃Z�b�g
	static void SetCamera(Camera* camera) { Object3d::camera_ = camera; }
	//���C�g�̃Z�b�g
	static void SetLight(LightGroup* light) { Object3d::light_ = light; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �J����
	static Camera* camera_;
	//���C�g
	static LightGroup* light_;

public: // �����o�֐�
	// �R���X�g���N�^
	Object3d() = default;
	// �f�X�g���N�^
	virtual ~Object3d();
	// ������
	virtual bool Initialize();
	// ���t���[������
	virtual void Update();
	// �`��
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);
	// �s��̍X�V
	void UpdateWorldMatrix();
	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position_; }
	// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	// X,Y,Z�����̎擾
	const XMFLOAT3& GetRotation() { return rotation_; }
	// X,Y,Z�����̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	// �X�P�[���̎擾
	const XMFLOAT3& GetScale() { return scale_; }
	// �X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	// ���f���̐ݒ�
	void SetModel(Model* model) { model_ = model; };
	// �r���{�[�h�̐ݒ�
	void SetBillboard(bool isBillboard) { isBillboard_ = isBillboard; }
	// ���[���h�s��̎擾
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	// �R���C�_�[�̃Z�b�g
	void SetCollider(BaseCollider* collider);
	// �Փˎ��R�[���o�b�N�֐�
	virtual void OnCollision(const CollisionInfo& info) {}
	// ���[���h���W���擾
	XMFLOAT3 GetWorldPosition();
	// ���f�����擾
	inline Model* GetModel() { return model_; }
	// �F�̐ݒ�
	void SetColor(XMFLOAT4 _color) { color_ = _color; }
	// �e�I�u�W�F�N�g�̐ݒ�
	void SetParent(Object3d* parent) { parent_ = parent; }
	void SetParent(FbxObject3d* parent) { fbxParent_ = parent; }
	// �擾����{�[���̖��O�̐ݒ�
	void SetBoneName(std::string boneName) { boneName_ = boneName; }
	// �v���~�e�B�u�`��̐ݒ�
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