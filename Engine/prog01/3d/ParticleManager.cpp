#include "ParticleManager.h"
#include "ShaderManager.h"
#include "BaseCalculate.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

std::unique_ptr<ParticleManager> ParticleManager::Create(ID3D12Device* device, Camera* camera, std::wstring fName)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ParticleManager* partMan = new ParticleManager(device, camera);
	if (partMan == nullptr)
	{
		return nullptr;
	}

	// ������
	partMan->Initialize(fName);

	return std::unique_ptr<ParticleManager>(partMan);
}

ParticleManager::~ParticleManager()
{
	vertBuff_.Reset();
	texBuff_.Reset();
	constBuff_.Reset();

	descHeap_.Reset();
}

void ParticleManager::Initialize(std::wstring fName)
{
	// nullptr�`�F�b�N
	assert(device_);

	HRESULT result;

	// �f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	// �e�N�X�`���ǂݍ���
	LoadTexture(fName);

	// ���f������
	CreateModel();

	// �萔�o�b�t�@�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);

	if (FAILED(result))
	{
		assert(0);
	}
}

void ParticleManager::Update()
{
	HRESULT result;

	//�������s�����p�[�e�B�N����S�폜
	particles_.remove_if
	(
		[](Particle& x)
		{
			return x.frame >= x.numFrame;
		}
	);

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator it = particles_.begin();
		it != particles_.end();
		it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		// �i�s�x��0�`1�͈̔͂Ɋ��Z
		float f = (float)it->numFrame / it->frame;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;
		//�X�P�[���̐��`���
		it->scale = it->startScale + (it->endScale - it->startScale) / f;
		// �J���[�̐��`���
		it->color.x = it->startColor.x + (it->endColor.x - it->startColor.x) / f;
		it->color.y = it->startColor.y + (it->endColor.y - it->startColor.y) / f;
		it->color.z = it->startColor.z + (it->endColor.z - it->startColor.z) / f;
	}

	//���_�o�b�t�@�փf�[�^�]��
	int vertCount = 0;
	VertexPos* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//�p�[�e�B�N���̏���1�����f
		for (std::forward_list<Particle>::iterator it = particles_.begin();
			it != particles_.end();
			it++)
		{
			//���W
			vertMap->pos = it->position;
			//�X�P�[��
			vertMap->scale = it->scale;
			//�F
			vertMap->color = it->color;
			//���̒��_��
			vertMap++;
			if (++vertCount >= VERTEX_COUNT)
			{
				break;
			}
		}
		vertBuff_->Unmap(0, nullptr);
	}

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->mat = camera_->GetViewProjectionMatrix(); //�s��̍���
	constMap->matBillboard = camera_->GetBillboardMatrix();
	constBuff_->Unmap(0, nullptr);
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	UINT drawNum = (UINT)std::distance(particles_.begin(), particles_.end());
	if (drawNum > VERTEX_COUNT)
	{
		drawNum = VERTEX_COUNT;
	}
	// �p�[�e�B�N����1���Ȃ��ꍇ
	if (drawNum == 0)
	{
		return;
	}

	// nullptr�`�F�b�N
	assert(cmdList);

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Particle"));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Particle"));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV_);
	// �`��R�}���h
	cmdList->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
	float startScale, float endScale, XMFLOAT4 startColor, XMFLOAT4 endColor)
{
	//���X�g�ɗv�f��ǉ�
	particles_.emplace_front();
	//�ǉ������v�f���Q��
	Particle& p = particles_.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.numFrame = life;
	p.startScale = startScale;
	p.endScale = endScale;
	p.startColor = startColor;
	p.endColor = endColor;
}

void ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//����
	if (FAILED(result))
	{
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager::LoadTexture(std::wstring fName)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	std::wstring name = L"Resources/" + fName + L".png";

	result = LoadFromWICFile
	(
		name.c_str(), WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	if (FAILED(result))
	{
		assert(0);
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuff_)
	);
	if (FAILED(result))
	{
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff_->WriteToSubresource
	(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result))
	{
		assert(0);
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);
	gpuDescHandleSRV_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texBuff_->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView
	(
		texBuff_.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV_
	);
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos)*VERTEX_COUNT),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPos)*VERTEX_COUNT;
	vbView_.StrideInBytes = sizeof(VertexPos);
}

ParticleManager::ParticleManager(ID3D12Device* device, Camera* camera)
{
	device_ = device;
	camera_ = camera;
}