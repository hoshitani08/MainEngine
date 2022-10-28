#include "Sprite.h"
#include "ShaderManager.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// �ÓI�����o�ϐ��̎���
ID3D12Device* Sprite::device = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
XMMATRIX Sprite::matProjection;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap;
ComPtr<ID3D12Resource> Sprite::texBuff[srvCount];

bool Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	Sprite::device = device;

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;

	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH
	(
		0.0f, (float)window_width,
		(float)window_height, 0.0f,
		0.0f, 1.0f
	);

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = srvCount;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

void Sprite::StaticFinalize()
{
	descHeap.Reset();

	for (auto& a : texBuff)
	{
		a.Reset();
	}
}

bool Sprite::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result;
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile
	(
		filename, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
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
		IID_PPV_ARGS(&texBuff[texnumber])
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff[texnumber]->WriteToSubresource
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
		return false;
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texBuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		texBuff[texnumber].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber, descriptorHandleIncrementSize)
	);

	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(Sprite::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	Sprite::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Sprite"));
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Sprite"));
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	// �R�}���h���X�g������
	Sprite::cmdList = nullptr;
}

std::unique_ptr<Sprite> Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// ���T�C�Y
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (texBuff[texNumber])
	{
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		// �X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Sprite�̃C���X�^���X�𐶐�
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!sprite->Initialize())
	{
		delete sprite;
		assert(0);
		return nullptr;
	}

	return std::unique_ptr<Sprite>(sprite);
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	position_ = position;
	size_ = size;
	anchorPoint_ = anchorpoint;
	matWorld_ = XMMatrixIdentity();
	color_ = color;
	texNumber_ = texNumber;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	texSize_ = size;
}

Sprite::~Sprite()
{
	vertBuff_.Reset();
	constBuff_.Reset();
}

bool Sprite::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
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
		return false;
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color_;
		constMap->mat = matProjection;
		constBuff_->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::SetRotation(float rotation)
{
	rotation_ = rotation;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	position_ = position;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetSize(XMFLOAT2 size)
{
	size_ = size;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint)
{
	anchorPoint_ = anchorpoint;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize)
{
	texBase_ = texBase;
	texSize_ = texSize;

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
}

void Sprite::Draw()
{
	// ���[���h�s��̍X�V
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color_;
		constMap->mat = matWorld_ * matProjection;	// �s��̍���	
		constBuff_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), texNumber_, descriptorHandleIncrementSize));
	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VertexPosUv vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	if (texBuff[texNumber_])
	{
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber_]->GetDesc();

		float tex_left = texBase_.x / resDesc.Width;
		float tex_right = (texBase_.x + texSize_.x) / resDesc.Width;
		float tex_top = texBase_.y / resDesc.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}
}