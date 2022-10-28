#include "LightGroup.h"

#include <assert.h>

using namespace DirectX;

// 静的メンバ変数の実体
ID3D12Device* LightGroup::device_ = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	LightGroup::device_ = device;
}

std::unique_ptr<LightGroup> LightGroup::Create()
{
	//3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
	instance->Initialize();

	return std::unique_ptr<LightGroup>(instance);
}

LightGroup::~LightGroup()
{
	constBuff_.Reset();
}

void LightGroup::Initialize()
{
	// nullptrチェック
	assert(device_);

	DefaultLightSetting();

	HRESULT result;
	// 定数バッファの生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);

	if (FAILED(result))
	{
		assert(0);
	}

	// 定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update()
{
	// 値の更新があった時だけ定数バッファに転送する
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		// 環境光
		constMap->ambientColor = ambientColor_;
		// 平行光源
		for (int i = 0; i < DIR_LIGHT_NUM; i++) {
			// ライトが有効なら設定を転送
			if (dirLights_[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights_[i].GetLightColor();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->dirLights[i].active = 0;
			}
		}
		// 点光源
		for (int i = 0; i < POINT_LIGHT_NUM; i++)
		{
			// ライトが有効なら設定を転送
			if (pointLights_[i].IsActive())
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightPos = pointLights_[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights_[i].GetLightColor();
				constMap->pointLights[i].lightAtten = pointLights_[i].GetLightAtten();
			}
			// ライトが無効ならライト色を0に
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}
		// スポットライト
		for (int i = 0; i < SPOT_LIGHT_NUM; i++)
		{
			// ライトが有効なら設定を転送
			if (spotLights_[i].IsActive())
			{
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -spotLights_[i].GetLightDir();
				constMap->spotLights[i].lightPos = spotLights_[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights_[i].GetLightColor();
				constMap->spotLights[i].lightAtten = spotLights_[i].GetLightAtten();
				constMap->spotLights[i].lightFactorAngleCos = spotLights_[i].GetLightFactorAngleCos();
			}
			// ライトが無効ならライト色を0に
			else
			{
				constMap->spotLights[i].active = 0;
			}
		}
		// 丸影
		for (int i = 0; i < CIRCLE_SHADOW_NUM; i++)
		{
			// 有効なら設定を転送
			if (circleShadows_[i].IsActive())
			{
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows_[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows_[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = circleShadows_[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows_[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows_[i].GetFactorAngleCos();
			}
			// 無効なら色を0に
			else
			{
				constMap->circleShadows[i].active = 0;
			}
		}

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[0].SetLightDir({ 0.0f, -1.0f, 0.0f, 0 });

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[1].SetLightDir({ +0.5f, +0.1f, +0.2f, 0 });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights_[2].SetLightDir({ -0.5f, +0.1f, -0.2f, 0 });
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);

	dirLights_[index].SetActive(active);

	dirty_ = true;
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);

	dirLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);

	dirLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);

	pointLights_[index].SetActive(active);

	dirty_ = true;
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);

	pointLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);

	pointLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);

	pointLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetActive(active);

	dirty_ = true;
}

void LightGroup::SetSpotLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetSpotLightPos(int index, const XMFLOAT3& lightpos)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetLightPos(lightpos);
	dirty_ = true;
}

void LightGroup::SetSpotLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < SPOT_LIGHT_NUM);

	spotLights_[index].SetLightFactorAngle(lightFactorAngle);
	dirty_ = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetActive(active);

	dirty_ = true;
}

void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetCasterPos(casterPos);
	dirty_ = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetDistanceCasterLight(distanceCasterLight);
	dirty_ = true;
}

void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetAtten(lightAtten);
	dirty_ = true;
}

void LightGroup::SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < CIRCLE_SHADOW_NUM);

	circleShadows_[index].SetFactorAngle(lightFactorAngle);
	dirty_ = true;
}