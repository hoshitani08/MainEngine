#include "PmxModel.h"
#include "PmxLoader.h"

#include <DirectXTex.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>

// 静的メンバ変数の実体
ID3D12Device* PmxModel::device = nullptr;

std::string WStringToString(std::wstring oWString)
{
	// wstring → SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// バッファの破棄
	delete[] cpMultiByte;

	// 変換結果を返す
	return(oRet);
}

bool PmxModel::Initialize()
{
	// 頂点バッファ ---------------------------
	HRESULT result;
	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビュー(VBV)の作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファ -------------------
	for (auto& a : surfaces)
	{
		indices.emplace_back(a.vertexIndex);
	}
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
	descHeapDesc.NumDescriptors = texturePaths.size(); // テクスチャ枚数
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));//生成

	// メッシュ -------------------------------
	meshes.resize(materials.size());
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		if (materials[i].colorMapTextureIndex != NO_DATA_FLAG)
		{
			// ファイルパスを結合
			string filepath = WStringToString(texturePaths[materials[i].colorMapTextureIndex]);
			wchar_t wfilepath[128];

			// ユニコード文字列に変換
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

			const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

			// リソース設定
			CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
			(
				metadata.format,
				metadata.width,
				(UINT)metadata.height,
				(UINT16)metadata.arraySize,
				(UINT16)metadata.mipLevels
			);

			// テクスチャ用バッファの生成
			result = device->CreateCommittedResource
			(
				&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
				D3D12_HEAP_FLAG_NONE,
				&texresDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
				nullptr,
				IID_PPV_ARGS(&texbuff)
			);
			if (FAILED(result))
			{
				assert(0);
			}

			// テクスチャバッファにデータ転送
			result = texbuff->WriteToSubresource
			(
				0,
				nullptr, // 全領域へコピー
				img->pixels, // 元データアドレス
				(UINT)img->rowPitch, // 1ラインサイズ
				(UINT)img->slicePitch // 1枚サイズ
			);
			if (FAILED(result))
			{
				assert(0);
			}

			// シェーダリソースビュー作成
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
			D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

			srvDesc.Format = resDesc.Format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
			srvDesc.Texture2D.MipLevels = 1;

			device->CreateShaderResourceView
			(
				texbuff.Get(), //ビューと関連付けるバッファ
				&srvDesc, //テクスチャ設定情報
				CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					descHeapSRV->GetCPUDescriptorHandleForHeapStart(), // ヒープの先頭アドレス
					0,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				)
			);

			//GPUハンドル取得
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

void PmxModel::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	PmxModel::device = device;
}

std::unique_ptr<PmxModel> PmxModel::CreateFromObject(const std::wstring& text, bool smoothing)
{
	// 3Dオブジェクトのインスタンスを生成
	PmxModel* model = new PmxModel();
	if (model == nullptr)
	{
		return nullptr;
	}

	model = PmxLoader::GetInstance()->loadPMX(text);

	// 初期化
	if (!model->Initialize())
	{
		delete model;
		assert(0);
		return nullptr;
	}
	return std::unique_ptr<PmxModel>(model);
}