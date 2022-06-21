#include "ParticleManager.h"
#include "ShaderManager.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

static const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

static const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}

std::unique_ptr<ParticleManager> ParticleManager::Create(ID3D12Device* device, Camera* camera)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ParticleManager* partMan = new ParticleManager(device, camera);
	if (partMan == nullptr)
	{
		return nullptr;
	}

	// ������
	partMan->Initialize();

	return std::unique_ptr<ParticleManager>(partMan);
}

ParticleManager::~ParticleManager()
{
	vertBuff.Reset();
	texbuff.Reset();
	constBuff.Reset();

	descHeap.Reset();
}

void ParticleManager::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result;

	// �f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	// �e�N�X�`���ǂݍ���
	LoadTexture();

	// ���f������
	CreateModel();

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
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
	particles.remove_if
	(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		// �i�s�x��0�`1�͈̔͂Ɋ��Z
		float f = (float)it->num_frame / it->frame;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;
		//�X�P�[���̐��`���
		it->scale = it->s_scale + (it->e_scale - it->s_scale) / f;
		// �J���[�̐��`���
		it->color.x = it->s_color.x + (it->e_color.x - it->s_color.x) / f;
		it->color.y = it->s_color.y + (it->e_color.y - it->s_color.y) / f;
		it->color.z = it->s_color.z + (it->e_color.z - it->s_color.z) / f;
	}

	//���_�o�b�t�@�փf�[�^�]��
	int vertCount = 0;
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//�p�[�e�B�N���̏���1�����f
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
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
			if (++vertCount >= vertexCount)
			{
				break;
			}
		}
		vertBuff->Unmap(0, nullptr);
	}

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = camera->GetViewProjectionMatrix(); //�s��̍���
	constMap->matBillboard = camera->GetBillboardMatrix();
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	UINT drawNum = (UINT)std::distance(particles.begin(), particles.end());
	if (drawNum > vertexCount)
	{
		drawNum = vertexCount;
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
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// �`��R�}���h
	cmdList->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
	float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color)
{
	//���X�g�ɗv�f��ǉ�
	particles.emplace_front();
	//�ǉ������v�f���Q��
	Particle& p = particles.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.s_color = start_color;
	p.e_color = end_color;
}

void ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result))
	{
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager::LoadTexture()
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile
	(
		L"Resources/effect1.png", WIC_FLAGS_NONE,
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
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	if (FAILED(result))
	{
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource
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
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos)*vertexCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos)*vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);
}

ParticleManager::ParticleManager(ID3D12Device* device, Camera* camera)
{
	this->device = device;
	this->camera = camera;
}