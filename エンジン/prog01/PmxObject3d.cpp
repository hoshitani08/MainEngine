#include "PmxObject3d.h"
#include "ShaderManager.h"

// �ÓI�����o�ϐ��̎���
ID3D12Device* PmxObject3d::device = nullptr;
Camera* PmxObject3d::camera = nullptr;
LightGroup* PmxObject3d::light = nullptr;

void PmxObject3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptr�`�F�b�N
	assert(device);

	PmxObject3d::device = device;
	PmxObject3d::camera = camera;

	PmxModel::StaticInitialize(device);
}

std::unique_ptr<PmxObject3d> PmxObject3d::Create(PmxModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PmxObject3d* pmxObject3d = new PmxObject3d();
	if (pmxObject3d == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!pmxObject3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		pmxObject3d->SetModel(model);
	}
	return std::unique_ptr<PmxObject3d>(pmxObject3d);
}

bool PmxObject3d::Initialize()
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
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(TexturedModelConstantBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	return true;
}

void PmxObject3d::Update()
{
	HRESULT result;

	//�s��̍X�V
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// �萔�o�b�t�@�փf�[�^�]��
	TexturedModelConstantBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->view = matViewProjection;
	constMap->world = matWorld;
	constMap->projection = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(50.0f * (XM_PI / 180.0f), static_cast<float>(1280) / static_cast<float>(720), 1.0f, 1000.0f)
	);
	constBuff->Unmap(0, nullptr);
}

void PmxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"PMX"));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"PMX"));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//���C�g�̕`��
	light->Draw(cmdList, 4);

	// ���f���`��
	model->Draw(cmdList);
}

void PmxObject3d::UpdateWorldMatrix()
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
	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
}