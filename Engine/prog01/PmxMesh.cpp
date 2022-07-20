#include "PmxMesh.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

// �ÓI�����o�ϐ��̎���
ID3D12Device* PmxMesh::device = nullptr;

void PmxMesh::StaticInitialize(ID3D12Device* device)
{
	PmxMesh::device = device;

	// �}�e���A���̐ÓI������
	PmxMaterial::StaticInitialize(device);
}

PmxMesh::~PmxMesh()
{
	vertBuff_.Reset();
	indexBuff_.Reset();
}

void PmxMesh::CreateBuffers()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices_.begin(), vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	if (FAILED(result))
	{
		assert(0);
		return;
	}

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());
	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices_.begin(), indices_.end(), indexMap);
		indexBuff_->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void PmxMesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���_�o�b�t�@���Z�b�g(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// �C���f�b�N�X�o�b�t�@���Z�b�g(IBV)
	cmdList->IASetIndexBuffer(&ibView_);
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(2, material_->GetGpuHandle());

	// �}�e���A���̒萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(1, material_->GetConstantBuffer()->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}
