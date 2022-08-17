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
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* partMan = new ParticleManager(device, camera);
	if (partMan == nullptr)
	{
		return nullptr;
	}

	// 初期化
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
	// nullptrチェック
	assert(device);

	HRESULT result;

	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// テクスチャ読み込み
	LoadTexture();

	// モデル生成
	CreateModel();

	// 定数バッファの生成
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
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

	//寿命が尽きたパーティクルを全削除
	particles.remove_if
	(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		// 進行度を0～1の範囲に換算
		float f = (float)it->num_frame / it->frame;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//スケールの線形補間
		it->scale = it->s_scale + (it->e_scale - it->s_scale) / f;
		// カラーの線形補間
		it->color.x = it->s_color.x + (it->e_color.x - it->s_color.x) / f;
		it->color.y = it->s_color.y + (it->e_color.y - it->s_color.y) / f;
		it->color.z = it->s_color.z + (it->e_color.z - it->s_color.z) / f;
	}

	//頂点バッファへデータ転送
	int vertCount = 0;
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++)
		{
			//座標
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
			if (++vertCount >= vertexCount)
			{
				break;
			}
		}
		vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = camera->GetViewProjectionMatrix(); //行列の合成
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
	// パーティクルが1つもない場合
	if (drawNum == 0)
	{
		return;
	}

	// nullptrチェック
	assert(cmdList);

	// パイプラインステートの設定
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Particle"));
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Particle"));
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド
	cmdList->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
	float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素を参照
	Particle& p = particles.front();
	//値のセット
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

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result))
	{
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager::LoadTexture()
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile
	(
		L"Resources/bubble.png", WIC_FLAGS_NONE,
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
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result))
	{
		assert(0);
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
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

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos)*vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);
}

ParticleManager::ParticleManager(ID3D12Device* device, Camera* camera)
{
	this->device = device;
	this->camera = camera;
}