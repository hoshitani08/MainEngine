#include "LoadScene.h"
#include "DirectXCommon.h"
#include "BaseCalculate.h"

#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

LoadScene::~LoadScene()
{
}

void LoadScene::Initialize()
{
	black_ = Sprite::Create(100, {});
	black_->SetSize({ 1280 , 720 });

	sphere_ = Sprite::Create(101, { 640, 360 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	sphere_->SetSize(sphere_->GetSize() / 2);

	loading_ = Sprite::Create(102, { 660, 480 }, { 1,1,1,1 }, { 0.5f, 0.5f });
}

void LoadScene::Finalize()
{
}

void LoadScene::Update()
{
	//半径は-100
	XMVECTOR v0 = { 0, 0, -100, 0 };
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(angle));
	rotM *= XMMatrixRotationZ(XMConvertToRadians(angle));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR bossTarget = { 700.0f, 330.0f, 0 };
	XMVECTOR v3 = bossTarget + v;
	XMFLOAT3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	XMFLOAT3 center = { bossTarget.m128_f32[0], bossTarget.m128_f32[1], bossTarget.m128_f32[2] };
	XMFLOAT3 pos = f;
	sphere_->SetPosition({ pos.x, pos.y });

	angle += 5.0f;
}

void LoadScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	black_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	sphere_->Draw();
	loading_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void LoadScene::EffectDraw()
{
}
