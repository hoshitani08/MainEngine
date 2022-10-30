#include "LoadScene.h"
#include "DirectXCommon.h"

LoadScene::~LoadScene()
{
}

void LoadScene::Initialize()
{
	black_ = Sprite::Create(100, {});
	black_->SetSize({ 1280 , 720 });
}

void LoadScene::Finalize()
{
}

void LoadScene::Update()
{
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

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void LoadScene::EffectDraw()
{
}
