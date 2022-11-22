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
	//���a��-100
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
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	black_->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�X�v���C�g�`��
#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	sphere_->Draw();
	loading_->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion �O�i�X�v���C�g�`��
}

void LoadScene::EffectDraw()
{
}
