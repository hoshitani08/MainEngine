#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

// �ÓI�����o�ϐ��̎���
ID3D12Device* Object3d::device_ = nullptr;
Camera* Object3d::camera_ = nullptr;
LightGroup* Object3d::light_ = nullptr;

void Object3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N
	assert(device);

	Object3d::device_ = device;
	Object3d::camera_ = camera;

	Model::StaticInitialize(device);
}

void Object3d::StaticFinalize()
{

}

std::unique_ptr<Object3d> Object3d::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Object3d* object3d = new Object3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!object3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		object3d->SetModel(model);
	}

	return std::unique_ptr<Object3d>(object3d);
}

Object3d::~Object3d()
{
	if (collider_)
	{
		//�R���W�����}�l�[�W������o�^����������
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		delete collider_;
	}
	constBuffB0_.Reset();
}

bool Object3d::Initialize()
{
	// nullptr�`�F�b�N
	assert(device_);
	//�N���X���̕�������擾
	name_ = typeid(*this).name();

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0_)
	);

	return true;
}

void Object3d::Update()
{
	HRESULT result;

	//�s��̍X�V
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera_->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera_->GetEye();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap0);
	constMap0->viewproj = matViewProjection;
	constMap0->world = matWorld_;
	constMap0->cameraPos = cameraPos;
	constMap0->color = color_;
	constBuffB0_->Unmap(0, nullptr);

	// �����蔻��X�V
	if (collider_)
	{
		collider_->Update();
	}
}

void Object3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptr�`�F�b�N
	assert(device_);

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Object", type_));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Object", type_));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());
	//���C�g�̕`��
	light_->Draw(cmdList, 3);
	//���f���`��
	model_->Draw(cmdList);
}

void Object3d::UpdateWorldMatrix()
{
	assert(camera_);

	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ���[���h�s��̍���
	if (isBillboard_ && camera_)
	{
		const XMMATRIX& matBillboard = camera_->GetBillboardMatrix();

		matWorld_ = XMMatrixIdentity();
		matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f
		matWorld_ *= matBillboard;
		matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	}
	else
	{
		matWorld_ = XMMatrixIdentity(); // �ό`�����Z�b�g
		matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f
		matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	}

	// �e�I�u�W�F�N�g�������
	if (parent_ != nullptr)
	{
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}
	if (fbxParent_ != nullptr)
	{
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		if (boneName_ != "")
		{
			matWorld_ *= fbxParent_->GetBoneMatWorld(boneName_) * fbxParent_->GetMatWorld();
		}
		else
		{
			matWorld_ *= fbxParent_->GetMatWorld();
		}
	}
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	collider_ = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(collider);
	//�s��̍X�V
	UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}

XMFLOAT3 Object3d::GetWorldPosition()
{
	XMFLOAT3 worldpos = {};
	worldpos.x = matWorld_.r[3].m128_f32[0];
	worldpos.y = matWorld_.r[3].m128_f32[1];
	worldpos.z = matWorld_.r[3].m128_f32[2];

	return worldpos;
}