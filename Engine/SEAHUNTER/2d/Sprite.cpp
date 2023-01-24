#include "Sprite.h"
#include "ShaderManager.h"

#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

// 静的メンバ変数の実体
ID3D12Device* Sprite::device_ = nullptr;
UINT Sprite::descriptorHandleIncrementSize_;
ID3D12GraphicsCommandList* Sprite::cmdList_ = nullptr;
XMMATRIX Sprite::matProjection_;
ComPtr<ID3D12DescriptorHeap> Sprite::descHeap_;
ComPtr<ID3D12Resource> Sprite::texBuff_[SRV_COUNT];

bool Sprite::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	Sprite::device_ = device;

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;

	// 射影行列計算
	matProjection_ = XMMatrixOrthographicOffCenterLH
	(
		0.0f, static_cast<float>(window_width),
		static_cast<float>(window_height), 0.0f,
		0.0f, 1.0f
	);

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = SRV_COUNT;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//生成
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

void Sprite::StaticFinalize()
{
	descHeap_.Reset();
	for (auto& a : texBuff_)
	{
		a.Reset();
	}
}

bool Sprite::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	// nullptrチェック
	assert(device_);

	HRESULT result;
	// WICテクスチャのロード
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
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff_[texnumber])
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// テクスチャバッファにデータ転送
	result = texBuff_[texnumber]->WriteToSubresource
	(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff_[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView
	(
		texBuff_[texnumber].Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), texnumber, descriptorHandleIncrementSize_)
	);

	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Sprite::cmdList_ == nullptr);

	// コマンドリストをセット
	Sprite::cmdList_ = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Sprite"));
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Sprite"));
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	// コマンドリストを解除
	Sprite::cmdList_ = nullptr;
}

std::unique_ptr<Sprite> Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 仮サイズ
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (texBuff_[texNumber])
	{
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texBuff_[texNumber]->GetDesc();
		// スプライトのサイズをテクスチャのサイズに設定
		size = { static_cast<float>(resDesc.Width), static_cast<float>(resDesc.Height) };
	}

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr)
	{
		return nullptr;
	}

	// 初期化
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
	defSize_ = size;
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
	// nullptrチェック
	assert(device_);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * VERT_NUM),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
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

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color_;
		constMap->mat = matProjection_;
		constBuff_->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::SetRotation(float rotation)
{
	rotation_ = rotation;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetPosition(XMFLOAT2 position)
{
	position_ = position;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetSize(XMFLOAT2 size)
{
	size_ = size;
	defSize_ = size;

	size_ = { defSize_.x * scale_, defSize_.y * scale_ };

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetScale(float scale)
{
	scale_ = scale;
	size_ = { defSize_.x * scale_, defSize_.y * scale_ };


	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetColor(XMFLOAT4 _color)
{
	color_ = _color;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint)
{
	anchorPoint_ = anchorpoint;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize)
{
	texBase_ = texBase;
	texSize_ = texSize;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::Draw()
{
	// ワールド行列の更新
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = color_;
		constMap->mat = matWorld_ * matProjection_;	// 行列の合成	
		constBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファの設定
	cmdList_->IASetVertexBuffers(0, 1, &vbView_);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	// デスクリプタヒープをセット
	cmdList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList_->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), texNumber_, descriptorHandleIncrementSize_));
	// 描画コマンド
	cmdList_->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[VERT_NUM] = {};

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (texBuff_[texNumber_])
	{
		D3D12_RESOURCE_DESC resDesc = texBuff_[texNumber_]->GetDesc();

		float tex_left = texBase_.x / resDesc.Width;
		float tex_right = (texBase_.x + texSize_.x) / resDesc.Width;
		float tex_top = texBase_.y / resDesc.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}
}