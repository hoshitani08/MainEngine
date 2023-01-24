#include "Mesh.h"

#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

// 静的メンバ変数の実体
ID3D12Device* Mesh::device_ = nullptr;

void Mesh::StaticInitialize(ID3D12Device* device)
{
	Mesh::device_ = device;

	// マテリアルの静的初期化
	Material::StaticInitialize(device);
}

Mesh::~Mesh()
{
	vertBuff_.Reset();
	indexBuff_.Reset();
}

void Mesh::SetName(const std::string& name)
{
	name_ = name;
}

void Mesh::AddVertex(const VertexPosNormalUv& vertex)
{
	vertices_.emplace_back(vertex);
}

void Mesh::AddIndex(unsigned short index)
{
	indices_.emplace_back(index);
}

void Mesh::SetMaterial(Material* material)
{
	material_ = material;
}

void Mesh::CreateBuffers()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());
	// 頂点バッファ生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	vertBuff_->SetName(L"meshvert");

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices_.begin(), vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	if (FAILED(result))
	{
		assert(0);
		return;
	}

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());
	// インデックスバッファ生成
	result = device_->CreateCommittedResource
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

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		std::copy(indices_.begin(), indices_.end(), indexMap);
		indexBuff_->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView_);

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(2, material_->GetGpuHandle());

	// マテリアルの定数バッファをセット
	ID3D12Resource* constBuff = material_->GetConstantBuffer();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Mesh::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData_[indexPosition].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVetexNormals()
{
	auto itr = smoothData_.begin();

	for (; itr != smoothData_.end(); ++itr)
	{
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全頂点の法線を平均する
		XMVECTOR normal = {};

		for (unsigned short index : v)
		{
			normal += XMVectorSet(vertices_[index].normal.x, vertices_[index].normal.y, vertices_[index].normal.z, 0);
		}

		normal = XMVector3Normalize(normal / static_cast<float>(v.size()));
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v)
		{
			vertices_[index].normal = { normal .m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}