#include "PmxObject3d.h"
#include "ShaderManager.h"

// 静的メンバ変数の実体
ID3D12Device* PmxObject3d::device = nullptr;
Camera* PmxObject3d::camera = nullptr;
LightGroup* PmxObject3d::light = nullptr;

void PmxObject3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptrチェック
	assert(device);

	PmxObject3d::device = device;
	PmxObject3d::camera = camera;

	PmxModel::StaticInitialize(device);
}

std::unique_ptr<PmxObject3d> PmxObject3d::Create(PmxModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	PmxObject3d* pmxObject3d = new PmxObject3d();
	if (pmxObject3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!pmxObject3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		pmxObject3d->SetModel(model);
	}
	return std::unique_ptr<PmxObject3d>(pmxObject3d);
}

bool PmxObject3d::Initialize()
{
	// nullptrチェック
	assert(device);
	//クラス名の文字列を取得
	name = typeid(*this).name();

	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(TexturedModelConstantBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	return true;
}

void PmxObject3d::Update()
{
	HRESULT result;

	//行列の更新
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// 定数バッファへデータ転送
	TexturedModelConstantBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->view = matViewProjection;
	constMap->world = matWorld;
	constMap->projection = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(50.0f * (XM_PI / 180.0f), static_cast<float>(1280) / static_cast<float>(720), 1.0f, 1000.0f)
	);
	constBuff->Unmap(0, nullptr);
}

void PmxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"PMX"));
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"PMX"));
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//ライトの描画
	light->Draw(cmdList, 4);

	// モデル描画
	model->Draw(cmdList);
}

void PmxObject3d::UpdateWorldMatrix()
{
	assert(camera);

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映
}