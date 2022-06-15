#include "PmxModel.h"
#include "PmxLoader.h"

bool PmxModel::Initialize(ID3D12Device* device)
{
	// 頂点バッファ ---------------------------
	HRESULT result;
	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(PMXModelData::Vertex) * allData.vertices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// 頂点バッファへのデータ転送
	PMXModelData::Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビュー(VBV)の作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// UV座標 ---------------------------------
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

	// 法線バッファ ---------------------------
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

	// インデックスバッファ -------------------
	// 頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビュー(IBV)の作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// SRV用デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = allData.texturePaths.size(); // テクスチャ枚数
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//生成

	// メッシュ -------------------------------
	meshes.resize(data.materials.size());
	D3DX11_IMAGE_LOAD_INFO loadInfo{};
	D3DX11_IMAGE_INFO imageInfo{};
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		if (data.materials[i].colorMapTextureIndex != PMXModelData::NO_DATA_FLAG)
		{
			// テクスチャ
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

			// テクスチャシェーダー
			createTexturedShader(_pDevice, meshes[i]);
		}
		else
		{
			// 単色シェーダー
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
	// 頂点バッファをセット(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット(IBV)
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}