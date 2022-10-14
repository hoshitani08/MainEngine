#include "PmxModel.h"
#include "PmxLoader.h"

#include <DirectXTex.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>

// �ÓI�����o�ϐ��̎���
ID3D12Device* PmxModel::device = nullptr;

std::string WStringToString(std::wstring oWString)
{
	// wstring �� SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpMultiByte;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

bool PmxModel::Initialize()
{
	// ���_�o�b�t�@ ---------------------------
	HRESULT result;
	// ���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[(VBV)�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@ -------------------
	for (auto& a : surfaces)
	{
		indices.emplace_back(a.vertexIndex);
	}
	// ���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = texturePaths.size(); // �e�N�X�`������
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//����

	// ���b�V�� -------------------------------
	meshes.resize(materials.size());
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		if (materials[i].colorMapTextureIndex != NO_DATA_FLAG)
		{
			// �t�@�C���p�X������
			string filepath = WStringToString(texturePaths[materials[i].colorMapTextureIndex]);
			wchar_t wfilepath[128];

			// ���j�R�[�h������ɕϊ�
			MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

			result = LoadFromWICFile
			(
				wfilepath, WIC_FLAGS_NONE,
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
				img->pixels, // ���f�[�^�A�h���X
				(UINT)img->rowPitch, // 1���C���T�C�Y
				(UINT)img->slicePitch // 1���T�C�Y
			);
			if (FAILED(result))
			{
				assert(0);
			}

			// �V�F�[�_���\�[�X�r���[�쐬
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
			D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

			srvDesc.Format = resDesc.Format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
			srvDesc.Texture2D.MipLevels = 1;

			device->CreateShaderResourceView
			(
				texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
				&srvDesc, //�e�N�X�`���ݒ���
				CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					descHeapSRV->GetCPUDescriptorHandleForHeapStart(), // �q�[�v�̐擪�A�h���X
					0,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				)
			);

			//GPU�n���h���擾
			gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
			(
				descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
				0,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);
		}

		meshes[i].vertexNum = materials[i].vertexNum;
		meshes[i].diffuseColor = materials[i].diffuse;
		meshes[i].specularColor = materials[i].specular;
		meshes[i].specularity = materials[i].specularity;
		meshes[i].ambientColor = materials[i].ambient;
	}

	return true;
}

void PmxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// ���_�o�b�t�@���Z�b�g(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@���Z�b�g(IBV)
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void PmxModel::StaticInitialize(ID3D12Device* device)
{
	// nullptr�`�F�b�N
	assert(device);

	PmxModel::device = device;
}

std::unique_ptr<PmxModel> PmxModel::CreateFromObject(const std::wstring& text, bool smoothing)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PmxModel* model = new PmxModel();
	if (model == nullptr)
	{
		return nullptr;
	}

	model = PmxLoader::GetInstance()->loadPMX(text);

	// ������
	if (!model->Initialize())
	{
		delete model;
		assert(0);
		return nullptr;
	}
	return std::unique_ptr<PmxModel>(model);
}