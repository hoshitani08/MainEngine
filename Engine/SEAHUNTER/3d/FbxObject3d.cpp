#include "FbxObject3d.h"
#include "ShaderManager.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

// �ÓI�����o�ϐ��̎���
ID3D12Device* FbxObject3d::device_ = nullptr;
Camera* FbxObject3d::camera_ = nullptr;
LightGroup* FbxObject3d::light_ = nullptr;

void FbxObject3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N
	assert(device);

	FbxObject3d::device_ = device;
	FbxObject3d::camera_ = camera;
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

	fbxObject3d->fName_ = HLSLfName;

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
	constBuffTransform_.Reset();
	constBuffSkin_.Reset();
}

bool FbxObject3d::Initialize()
{
	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_)
	);

	// �萔�o�b�t�@(�X�L��)�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin_)
	);

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime_.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	// �萔�o�b�t�@(�X�L��)�փf�[�^�]��
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMatSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin_->Unmap(0, nullptr);

	return true;
}

void FbxObject3d::Update()
{
	XMMATRIX matScale = {}, matRot = {}, matTrans = {};

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ���[���h�s��̍���
	matWorld_ = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection = camera_->GetViewProjectionMatrix();
	// ���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model_->GetModelTransform();
	// �J�������W
	const XMFLOAT3& cameraPos = camera_->GetEye();

	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld_;
		constMap->cameraPos = cameraPos;
		constBuffTransform_->Unmap(0, nullptr);
	}

	//�A�j���[�V����
	if (isPlay_)
	{
		if (animationEaseData_ != nullptr && animationEaseData_->GetActFlag())
		{
			animationEaseData_->Update();
			float start = static_cast<float>(startTime_.GetFrameCount());
			float end = static_cast<float>(endTime_.GetFrameCount());

			currentTime_.SetFrame((FbxLongLong)Ease::Action(EaseType::In, EaseFunctionType::Linear, start, end, animationEaseData_->GetTimeRate()));
		}
		else
		{
			//1�t���[���i�߂�
			currentTime_ += frameTime_;
		}

		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime_ > endTime_ && isLoop_)
		{
			currentTime_ = startTime_;
		}
		//���[�v���Ȃ��Ƃ��̓A�j���[�V�������I��
		else if (currentTime_ > endTime_)
		{
			currentTime_ = endTime_;
		}
	}

	//�{�[���z��
	std::vector<FbxModel::Bone>& bones = model_->GetBones();

	// �萔�o�b�t�@(�X�L��)�փf�[�^�]��
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime_);

		XMFLOAT3 scale = ToXMFLOAT3(fbxCurrentPose.GetS());
		XMFLOAT4 rotate = ToXMFLOAT4(fbxCurrentPose.GetQ());
		XMFLOAT3 translate = ToXMFLOAT3(fbxCurrentPose.GetT());

		//XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMatSkin->bones[i] = model_->GetModelTransform() * bones[i].invInitialPose * matCurrentPose * XMMatrixInverse(nullptr, model_->GetModelTransform());
	}
	constBuffSkin_->Unmap(0, nullptr);
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model_ == nullptr)
	{
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(fName_));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(fName_));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin_->GetGPUVirtualAddress());

	//���C�g�̕`��
	light_->Draw(cmdList, 4);

	// ���f���`��
	model_->Draw(cmdList);
}

void FbxObject3d::LoadAnimation()
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//�A�j���[�V�����J�E���g
	int sceneCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < sceneCount; i++)
	{
		//���f�[�^
		Animation tempData = {};
		//i�Ԃ̃A�j���[�V�����擾
		tempData.animstack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//�A�j���[�V�����̖��O���擾
		const char* animstackname = tempData.animstack->GetName();
		//�A�j���[�V�����̎��ԏ��
		tempData.takeinfo = fbxScene->GetTakeInfo(animstackname);
		//���f�[�^�����f�[�^�ɓ����
		animationData_.push_back(tempData);
	}
}

void FbxObject3d::PlayAnimation(int animationNumber, bool isLoop)
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//�A�j���[�V�����̕ύX
	fbxScene->SetCurrentAnimationStack(animationData_[animationNumber].animstack);
	//�J�n���Ԏ擾
	startTime_ = animationData_[animationNumber].takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime_ = animationData_[animationNumber].takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime_ = startTime_;
	//�Đ�����Ԃɂ���
	isPlay_ = true;
	//���[�v�����邩
	isLoop_ = isLoop;

	if (animationEaseData_ != nullptr)
	{
		animationEaseData_->Reset();
	}
}

XMFLOAT3 FbxObject3d::GetWorldPosition()
{
	XMFLOAT3 worldpos = {};
	worldpos.x = matWorld_.r[3].m128_f32[0];
	worldpos.y = matWorld_.r[3].m128_f32[1];
	worldpos.z = matWorld_.r[3].m128_f32[2];

	return worldpos;
}

const XMMATRIX FbxObject3d::GetBoneMatWorld(std::string name)
{
	//�{�[���z��
	std::vector<FbxModel::Bone>& bone = model_->GetBones();
	XMMATRIX mat = {};

	for (int i = 0; i < bone.size(); i++)
	{
		if (bone[i].name == name)
		{
			//���̎p���s����擾
			FbxAMatrix fbxCurrentPose = bone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime_);
			//XMMATRIX�ɕϊ�
			FbxLoader::ConvertMatrixFromFbx(&mat, fbxCurrentPose);
			//����
			return model_->GetModelTransform() * mat;
		}
	}

	return {};
}

XMFLOAT3 FbxObject3d::ToXMFLOAT3(FbxVector4 count)
{
	XMFLOAT3 temp = {};
	temp.x = static_cast<float>(count[0]);
	temp.y = static_cast<float>(count[1]);
	temp.z = static_cast<float>(count[2]);

	return temp;
}

XMFLOAT4 FbxObject3d::ToXMFLOAT4(FbxQuaternion count)
{
	XMFLOAT4 temp = {};
	temp.x = static_cast<float>(count[0]);
	temp.y = static_cast<float>(count[1]);
	temp.z = static_cast<float>(count[2]);
	temp.w = static_cast<float>(count[3]);

	return temp;
}