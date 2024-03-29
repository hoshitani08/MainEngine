#include "FbxModel.h"

void FbxModel::CreateBuffers(ID3D12Device* device)
{
	HRESULT result;
	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices_.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));

	// 頂点バッファへのデータ転送
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビュー(VBV)の作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);
	// 頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices_.begin(), indices_.end(), indexMap);
		indexBuff_->Unmap(0, nullptr);
	}

	// インデックスバッファビュー(IBV)の作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

	// SRV用デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = MAX_TEXTURES; // テクスチャ枚数
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV_));//生成

	CreateTexture(baseTexture_, device, 0);
	CreateTexture(normalTexture_, device, 1);
	CreateTexture(metalnessTexture_, device, 2);
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファをセット(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// インデックスバッファをセット(IBV)
	cmdList->IASetIndexBuffer(&ibView_);

	// デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void FbxModel::CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex)
{
	if (texture.scratchImg.GetImageCount() == 0)
	{
		return;
	}

	HRESULT result;
	// テクスチャ画像データ
	const DirectX::Image* img = texture.scratchImg.GetImage(0, 0, 0); // 生データ抽出
	assert(img);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		texture.metaData.format,
		texture.metaData.width,
		(UINT)texture.metaData.height,
		(UINT16)texture.metaData.arraySize,
		(UINT16)texture.metaData.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texture.texBuff)
	);

	// テクスチャバッファにデータ転送
	result = texture.texBuff->WriteToSubresource
	(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);

	// シェーダリソースビュー(SRV)作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texture.texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		texture.texBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart(), // ヒープの先頭アドレス
			srvIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

	//GPUハンドル取得
	texture.gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		descHeapSRV_->GetGPUDescriptorHandleForHeapStart(),
		srvIndex,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
}

FbxModel::~FbxModel()
{
	//FBXシーン解放
	fbxScene_->Destroy();

	vertBuff_.Reset();
	indexBuff_.Reset();
	texBuff_.Reset();
}