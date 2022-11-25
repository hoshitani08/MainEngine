#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "LightGroup.h"

class FbxObject3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 200;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i�X�L�j���O�j
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	// �A�j���[�V�����p�f�[�^�\����
	struct Animation
	{
		FbxAnimStack* animstack;
		FbxTakeInfo* takeinfo;
	};

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
	/// <param name="HLSLfName">HLSL��</param>
	/// <param name="isAnimation">�A�j���[�V�����t���O</param>
	/// <returns></returns>
	static std::unique_ptr<FbxObject3d> Create(FbxModel* model = nullptr, std::wstring HLSLfName = L"BasicFBX", bool isAnimation = false);
	/// <summary>
	/// �f�o�C�X�̐ݒ�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device_ = device; }
	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera) { FbxObject3d::camera_ = camera; }
	/// <summary>
	/// ���C�g�̐ݒ�
	/// </summary>
	/// <param name="light">���C�g</param>
	static void SetLight(LightGroup* light) { FbxObject3d::light_ = light; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �J����
	static Camera* camera_;
	//���C�g
	static LightGroup* light_;

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FbxObject3d();
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize();
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
	/// ���f����ݒ�
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(FbxModel* model) { model_ = model; }
	/// <summary>
	/// �A�j���[�V�����̃��[�h
	/// </summary>
	void LoadAnimation();
	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	/// <param name="animationNumber">�A�j���[�V�����n���h��</param>
	/// <param name="isLoop">���[�v�����邩</param>
	void PlayAnimation(int animationNumber = 0, bool isLoop = true);
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
	/// <returns>X,Y,Z�����̊p�x</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }
	/// <summary>
	/// X,Y,Z�����̐ݒ�
	/// </summary>
	/// <param name="rotation">X,Y,Z�����p�x</param>
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
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	/// <summary>
	/// �{�[���s��̎擾
	/// </summary>
	/// <param name="name">�{�[����</param>
	/// <returns>�{�[���s��</returns>
	const XMMATRIX GetBoneMatWorld(std::string name);
	/// <summary>
	/// �A�j���[�V�������I�������
	/// </summary>
	/// <returns></returns>
	bool AnimationEnd() { return currentTime_ >= endTime_; }

protected: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform_;
	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin_;
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position_ = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld_ = {};
	// ���f��
	FbxModel* model_ = nullptr;
	//1�t���[���̎���
	FbxTime frameTime_ = {};
	//�A�j���[�V�����J�n����
	FbxTime startTime_ = {};
	//�A�j���[�V�����I������
	FbxTime endTime_ = {};
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime_ = {};
	//�A�j���[�V�����Đ���
	bool isPlay_ = false;
	//���[�v�t���O
	bool isLoop_ = false;
	//�A�j���[�V�����̃f�[�^
	std::vector<Animation> animationData_ = {};
	// �V�F�[�_�t�@�C����
	std::wstring fName_ = L"";
};