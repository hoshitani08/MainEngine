#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "PmxModel.h"
#include "Camera.h"
#include "LightGroup.h"

class PmxObject3d
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
	// texturedModel�V�F�[�_�[�̒萔�o�b�t�@�f�[�^�^
	struct TexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// notTexturedModel�V�F�[�_�[�̒萔�o�b�t�@�f�[�^�^
	struct NotTexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMVECTOR diffuseColor;
		XMVECTOR ambientColor;
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferData
	{
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<PmxObject3d> Create(PmxModel* model = nullptr);
	// setter
	static void SetDevice(ID3D12Device* device) { PmxObject3d::device = device; }
	static void SetCamera(Camera* camera) { PmxObject3d::camera = camera; }
	static void SetLight(LightGroup* light) { PmxObject3d::light = light; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �J����
	static Camera* camera;
	//���C�g
	static LightGroup* light;

public: // �����o�֐�
	// ������
	bool Initialize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// �s��̍X�V
	void UpdateWorldMatrix();
	//���f����ݒ�
	void SetModel(PmxModel* _model) { model = _model; }

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 _position) { position = _position; }
	// X,Y,Z�����̎擾
	const XMFLOAT3& GetRotation() { return rotation; }
	// X,Y,Z�����̐ݒ�
	void SetRotation(XMFLOAT3 _rotation) { rotation = _rotation; }
	// �X�P�[���̎擾
	const XMFLOAT3& GetScale() { return scale; }
	// �X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 _scale) { scale = _scale; }

protected: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// ���f��
	PmxModel* model = nullptr;
	//�N���X��(�f�o�b�N�p)
	const char* name = nullptr;
};

