#include "Object3d.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "ShaderManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// �ÓI�����o�ϐ��̎���
ID3D12Device* Object3d::device = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
Camera* Object3d::camera = nullptr;
LightGroup* Object3d::light = nullptr;

void Object3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N
	assert(device);

	Object3d::device = device;
	Object3d::camera = camera;

	Model::StaticInitialize(device);
}

void Object3d::StaticFinalize()
{

}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(Object3d::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	Object3d::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Object"));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Object"));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	// �R�}���h���X�g������
	Object3d::cmdList = nullptr;
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
	if (collider)
	{
		//�R���W�����}�l�[�W������o�^����������
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
	constBuffB0.Reset();
}

bool Object3d::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);
	//�N���X���̕�������擾
	name = typeid(*this).name();

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	return true;
}

void Object3d::Update()
{
	HRESULT result;

	//�s��̍X�V
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
	constMap0->viewproj = matViewProjection;
	constMap0->world = matWorld;
	constMap0->cameraPos = cameraPos;
	constBuffB0->Unmap(0, nullptr);

	// �����蔻��X�V
	if (collider)
	{
		collider->Update();
	}
}

void Object3d::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(Object3d::cmdList);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//���C�g�̕`��
	light->Draw(cmdList, 3);
	//���f���`��
	model->Draw(cmdList);
}

void Object3d::UpdateWorldMatrix()
{
	assert(camera);

	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s��̍���
	if (isBillboard && camera)
	{
		const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

		matWorld = XMMatrixIdentity();
		matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
		matWorld *= matBillboard;
		matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	}
	else
	{
		matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
		matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
		matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	}

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(collider);
	//�s��̍X�V
	UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}

XMFLOAT3 Object3d::GetWorldPosition()
{
	XMFLOAT3 worldpos;

	worldpos.x = matWorld.r[3].m128_f32[0];
	worldpos.y = matWorld.r[3].m128_f32[1];
	worldpos.z = matWorld.r[3].m128_f32[2];

	return worldpos;
}