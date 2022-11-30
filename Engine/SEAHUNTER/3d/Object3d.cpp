#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

// 静的メンバ変数の実体
ID3D12Device* Object3d::device_ = nullptr;
Camera* Object3d::camera_ = nullptr;
LightGroup* Object3d::light_ = nullptr;

void Object3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptrチェック
	assert(device);

	Object3d::device_ = device;
	Object3d::camera_ = camera;

	Model::StaticInitialize(device);
}

void Object3d::StaticFinalize()
{

}

std::unique_ptr<Object3d> Object3d::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3d* object3d = new Object3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		object3d->SetModel(model);
	}

	return std::unique_ptr<Object3d>(object3d);
}

Object3d::~Object3d()
{
	if (collider_)
	{
		//コリジョンマネージャから登録を解除する
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		delete collider_;
	}
	constBuffB0_.Reset();
}

bool Object3d::Initialize()
{
	// nullptrチェック
	assert(device_);
	//クラス名の文字列を取得
	name_ = typeid(*this).name();

	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0_)
	);

	return true;
}

void Object3d::Update()
{
	HRESULT result;

	//行列の更新
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera_->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera_->GetEye();

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap0 = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap0);
	constMap0->viewproj = matViewProjection;
	constMap0->world = matWorld_;
	constMap0->cameraPos = cameraPos;
	constMap0->color = color_;
	constBuffB0_->Unmap(0, nullptr);

	// 当たり判定更新
	if (collider_)
	{
		collider_->Update();
	}
}

void Object3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptrチェック
	assert(device_);

	// パイプラインステートの設定
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(L"Object", type_));
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(L"Object", type_));
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());
	//ライトの描画
	light_->Draw(cmdList, 3);
	//モデル描画
	model_->Draw(cmdList);
}

void Object3d::UpdateWorldMatrix()
{
	assert(camera_);

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	if (isBillboard_ && camera_)
	{
		const XMMATRIX& matBillboard = camera_->GetBillboardMatrix();

		matWorld_ = XMMatrixIdentity();
		matWorld_ *= matScale; // ワールド行列にスケーリングを反映
		matWorld_ *= matRot; // ワールド行列に回転を反映
		matWorld_ *= matBillboard;
		matWorld_ *= matTrans; // ワールド行列に平行移動を反映
	}
	else
	{
		matWorld_ = XMMatrixIdentity(); // 変形をリセット
		matWorld_ *= matScale; // ワールド行列にスケーリングを反映
		matWorld_ *= matRot; // ワールド行列に回転を反映
		matWorld_ *= matTrans; // ワールド行列に平行移動を反映
	}

	// 親オブジェクトがあれば
	if (parent_ != nullptr)
	{
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}
	if (fbxParent_ != nullptr)
	{
		// 親オブジェクトのワールド行列を掛ける
		if (boneName_ != "")
		{
			matWorld_ *= fbxParent_->GetBoneMatWorld(boneName_) * fbxParent_->GetMatWorld();
		}
		else
		{
			matWorld_ *= fbxParent_->GetMatWorld();
		}
	}
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	collider_ = collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	UpdateWorldMatrix();
	//コライダーを更新しておく
	collider->Update();
}

XMFLOAT3 Object3d::GetWorldPosition()
{
	XMFLOAT3 worldpos = {};
	worldpos.x = matWorld_.r[3].m128_f32[0];
	worldpos.y = matWorld_.r[3].m128_f32[1];
	worldpos.z = matWorld_.r[3].m128_f32[2];

	return worldpos;
}