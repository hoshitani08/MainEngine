#include "FbxObject3d.h"
#include "ShaderManager.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

// �ÓI�����o�ϐ��̎���
ID3D12Device* FbxObject3d::device = nullptr;
Camera* FbxObject3d::camera = nullptr;
LightGroup* FbxObject3d::light = nullptr;

void FbxObject3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N
	assert(device);

	FbxObject3d::device = device;
	FbxObject3d::camera = camera;
}

void FbxObject3d::StaticFinalize()
{
	
}

std::unique_ptr<FbxObject3d> FbxObject3d::Create(FbxModel* model, std::wstring HLSLfName, bool isAnimation)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	FbxObject3d* fbxObject3d = new FbxObject3d();
	if (fbxObject3d == nullptr)
	{
		return nullptr;
	}

	fbxObject3d->fName = HLSLfName;

	// ������
	if (!fbxObject3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		fbxObject3d->SetModel(model);
	}

	if (isAnimation)
	{
		fbxObject3d->LoadAnimation();
	}

	return std::unique_ptr<FbxObject3d>(fbxObject3d);
}

FbxObject3d::~FbxObject3d()
{
	constBuffTransform.Reset();
	constBuffSkin.Reset();
}

bool FbxObject3d::Initialize()
{
	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	// �萔�o�b�t�@(�X�L��)�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	// �萔�o�b�t�@(�X�L��)�փf�[�^�]��
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMatSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);

	return true;
}

void FbxObject3d::Update()
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s��̍���
	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	// ���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	// �J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<FbxModel::Bone>& bones = model->GetBones();

	//�A�j���[�V����
	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime;

		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	// �萔�o�b�t�@(�X�L��)�փf�[�^�]��
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMatSkin->bones[i] = model->GetModelTransform() * bones[i].invInitialPose * matCurrentPose * XMMatrixInverse(nullptr, model->GetModelTransform());
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr)
	{
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(fName));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(fName));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//���C�g�̕`��
	light->Draw(cmdList, 4);

	// ���f���`��
	model->Draw(cmdList);
}

void FbxObject3d::LoadAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//�A�j���[�V�����J�E���g
	int sceneCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < sceneCount; i++)
	{
		//���f�[�^
		Animation tempData;
		//i�Ԃ̃A�j���[�V�����擾
		tempData.animstack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//�A�j���[�V�����̖��O���擾
		const char* animstackname = tempData.animstack->GetName();
		//�A�j���[�V�����̎��ԏ��
		tempData.takeinfo = fbxScene->GetTakeInfo(animstackname);
		//���f�[�^�����f�[�^�ɓ����
		animationData.push_back(tempData);
	}
}

void FbxObject3d::PlayAnimation(int animationNumber)
{
	FbxScene* fbxScene = model->GetFbxScene();
	//�A�j���[�V�����̕ύX
	fbxScene->SetCurrentAnimationStack(animationData[animationNumber].animstack);
	//�J�n���Ԏ擾
	startTime = animationData[animationNumber].takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = animationData[animationNumber].takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}