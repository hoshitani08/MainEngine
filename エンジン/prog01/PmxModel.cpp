#include "PmxModel.h"
#include "PmxLoader.h"

bool PmxModel::Initialize(ID3D12Device* device)
{
	// ���_�o�b�t�@ ---------------------------
	HRESULT result;
	// ���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(PMXModelData::Vertex) * allData.vertices.size());
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	PMXModelData::Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[(VBV)�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// UV���W ---------------------------------
	bufferDesc.ByteWidth = sizeof(XMFLOAT2) * allData.vertices.size();
	std::vector<XMFLOAT2> uv;
	uv.resize(data.vertices.size());
	for (unsigned i = 0; i < uv.size(); i++)
	{
		uv[i] = data.vertices[i].uv;
	}
	subresourceData.pSysMem = &uv[0];
	result = _pDevice->CreateBuffer(&bufferDesc, &subresourceData, &pVertexBuffers[UV]);
	if (FAILED(result))
	{
		return false;
	}

	// �@���o�b�t�@ ---------------------------
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * data.vertices.size();
	for (unsigned i = 0; i < vertices.size(); i++)
	{
		vertices[i] = data.vertices[i].normal;
	}
	subresourceData.pSysMem = &vertices[0];
	result = _pDevice->CreateBuffer(&bufferDesc, &subresourceData, &pVertexBuffers[NORMAL]);
	if (FAILED(result))
	{
		return false;
	}

	// �C���f�b�N�X�o�b�t�@ -------------------
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
	descHeapDesc.NumDescriptors = allData.texturePaths.size(); // �e�N�X�`������
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//����

	// ���b�V�� -------------------------------
	meshes.resize(data.materials.size());
	D3DX11_IMAGE_LOAD_INFO loadInfo{};
	D3DX11_IMAGE_INFO imageInfo{};
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		if (data.materials[i].colorMapTextureIndex != PMXModelData::NO_DATA_FLAG)
		{
			// �e�N�X�`��
			result = D3DX11GetImageInfoFromFileW(data.texturePaths[data.materials[i].colorMapTextureIndex].c_str(), NULL, &imageInfo, NULL);
			if (FAILED(result))
			{
				return false;
			}
			loadInfo.Width = imageInfo.Width;
			loadInfo.Height = imageInfo.Height;
			loadInfo.Depth = imageInfo.Depth;
			loadInfo.FirstMipLevel = 0;
			loadInfo.MipLevels = imageInfo.MipLevels;
			loadInfo.Usage = D3D11_USAGE_DEFAULT;
			loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			loadInfo.CpuAccessFlags = NULL;
			loadInfo.MiscFlags = NULL;
			loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			loadInfo.Filter = D3DX11_FILTER_LINEAR | D3DX11_FILTER_SRGB_IN;
			loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
			loadInfo.pSrcInfo = &imageInfo;

			result = D3DX11CreateShaderResourceViewFromFileW(_pDevice, data.texturePaths[data.materials[i].colorMapTextureIndex].c_str(), &loadInfo, NULL, &meshes[i].pTexture, nullptr);
			if (FAILED(result))
			{
				return false;
			}

			// �e�N�X�`���V�F�[�_�[
			createTexturedShader(_pDevice, meshes[i]);
		}
		else
		{
			// �P�F�V�F�[�_�[
			createNotTexturedShader(_pDevice, meshes[i]);
		}

		meshes[i].vertexNum = data.materials[i].vertexNum;
		meshes[i].diffuseColor = data.materials[i].diffuse;
		meshes[i].specularColor = data.materials[i].specular;
		meshes[i].specularity = data.materials[i].specularity;
		meshes[i].ambientColor = data.materials[i].ambient;
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