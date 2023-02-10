#include "FbxObject3d.h"
#include "ShaderManager.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

// 静的メンバ変数の実体
ID3D12Device* FbxObject3d::device_ = nullptr;
Camera* FbxObject3d::camera_ = nullptr;
LightGroup* FbxObject3d::light_ = nullptr;

void FbxObject3d::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptrチェック
	assert(device);

	FbxObject3d::device_ = device;
	FbxObject3d::camera_ = camera;
}

void FbxObject3d::StaticFinalize()
{
	
}

std::unique_ptr<FbxObject3d> FbxObject3d::Create(FbxModel* model, std::wstring HLSLfName, bool isAnimation)
{
	// 3Dオブジェクトのインスタンスを生成
	FbxObject3d* fbxObject3d = new FbxObject3d();
	if (fbxObject3d == nullptr)
	{
		return nullptr;
	}

	fbxObject3d->fName_ = HLSLfName;

	// 初期化
	if (!fbxObject3d->Initialize())
	{
		assert(0);
		return nullptr;
	}

	if (model)
	{
		fbxObject3d->SetModel(model);
	}

	if (isAnimation)
	{
		fbxObject3d->LoadAnimation();
	}

	return std::unique_ptr<FbxObject3d>(fbxObject3d);
}

FbxObject3d::~FbxObject3d()
{
	constBuffTransform_.Reset();
	constBuffSkin_.Reset();
}

bool FbxObject3d::Initialize()
{
	HRESULT result;
	// 定数バッファの生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_)
	);

	// 定数バッファ(スキン)の生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin_)
	);

	//1フレーム分の時間を60FPSで設定
	frameTime_.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	// 定数バッファ(スキン)へデータ転送
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMatSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin_->Unmap(0, nullptr);

	interpolationEase = std::make_unique<EaseData>(5);
	interpolationEase->SetActFlag(false);

	return true;
}

void FbxObject3d::Update()
{
	XMMATRIX matScale = {}, matRot = {}, matTrans = {};

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	matWorld_ *= matRot; // ワールド行列に回転を反映
	matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	// ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera_->GetViewProjectionMatrix();
	// モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model_->GetModelTransform();
	// カメラ座標
	const XMFLOAT3& cameraPos = camera_->GetEye();

	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld_;
		constMap->cameraPos = cameraPos;
		constBuffTransform_->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay_ && interpolationModel_ == nullptr)
	{
		if (animationEaseData_ != nullptr && animationEaseData_->GetActFlag())
		{
			animationEaseData_->Update();
			float start = static_cast<float>(startTime_.GetFrameCount());
			float end = static_cast<float>(endTime_.GetFrameCount());

			currentTime_.SetFrame((FbxLongLong)Ease::Action(EaseType::In, EaseFunctionType::Linear, start, end, animationEaseData_->GetTimeRate()));
		}
		else
		{
			//1フレーム進める
			currentTime_ += frameTime_;
		}

		//最後まで再生したら先頭に戻す
		if (currentTime_ > endTime_ && isLoop_)
		{
			currentTime_ = startTime_;
		}
		//ループしないときはアニメーションを終了
		else if (currentTime_ > endTime_)
		{
			currentTime_ = endTime_;
		}
	}

	//ボーン配列
	std::vector<FbxModel::Bone>& bones = model_->GetBones();

	std::vector<FbxModel::Bone>& interpolationBones = model_->GetBones();
	if (interpolationModel_ != nullptr)
	{
		interpolationBones = interpolationModel_->GetBones();
	}

	// 定数バッファ(スキン)へデータ転送
	ConstBufferDataSkin* constMatSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMatSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//今の姿勢行列
		XMMATRIX matCurrentPose = XMMatrixIdentity();
		//今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime_);

		if (!interpolationEase->GetEndFlag() && interpolationModel_ != nullptr)
		{
			FbxAMatrix interpolationFbxCurrentPose = interpolationBones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(interpolationTime_);

			XMFLOAT3 scaleF = ToXMFLOAT3(fbxCurrentPose.GetS());
			XMFLOAT4 rotateF = ToXMFLOAT4(fbxCurrentPose.GetQ());
			XMFLOAT3 translateF = ToXMFLOAT3(fbxCurrentPose.GetT());

			XMFLOAT3 interpolationScaleF = ToXMFLOAT3(interpolationFbxCurrentPose.GetS());
			XMFLOAT4 interpolationRotateF = ToXMFLOAT4(interpolationFbxCurrentPose.GetQ());
			XMFLOAT3 interpolationTranslateF = ToXMFLOAT3(interpolationFbxCurrentPose.GetT());

			// 補間の計算
			XMVECTOR scaleV = XMLoadFloat3(&scaleF);
			XMVECTOR rotateV = XMLoadFloat4(&rotateF);
			XMVECTOR translateV = XMLoadFloat3(&translateF);

			XMVECTOR interpolationScaleV = XMLoadFloat3(&interpolationScaleF);
			XMVECTOR interpolationRotateV = XMLoadFloat4(&interpolationRotateF);
			XMVECTOR interpolationTranslateV = XMLoadFloat3(&interpolationTranslateF);

			scaleV = XMVectorLerp(interpolationScaleV, scaleV, interpolationEase->GetTimeRate());
			rotateV = XMVectorLerp(interpolationRotateV, rotateV, interpolationEase->GetTimeRate());
			translateV = XMVectorLerp(interpolationTranslateV, translateV, interpolationEase->GetTimeRate());

			// スケール、回転、平行移動行列の計算
			XMMATRIX matScaling, matRotation, matTranslation;
			matScaling = XMMatrixScalingFromVector(scaleV);
			matRotation = XMMatrixRotationQuaternion(rotateV);
			matTranslation = XMMatrixTranslationFromVector(translateV);

			// 変形行列の計算
			matCurrentPose *= matScaling; // ワールド行列にスケーリングを反映
			matCurrentPose *= matRotation; // ワールド行列に回転を反映
			matCurrentPose *= matTranslation; // ワールド行列に平行移動を反映
		}
		else
		{
			//XMMATRIXに変換
			FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		}

		//合成してスキニング行列に
		constMatSkin->bones[i] = model_->GetModelTransform() * bones[i].invInitialPose * matCurrentPose * XMMatrixInverse(nullptr, model_->GetModelTransform());
	}
	constBuffSkin_->Unmap(0, nullptr);

	interpolationEase->Update();
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// モデルの割り当てがなければ描画しない
	if (model_ == nullptr)
	{
		return;
	}

	// パイプラインステートの設定
	cmdList->SetPipelineState(ShaderManager::GetInstance()->GetPipelineState(fName_));
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(ShaderManager::GetInstance()->GetRootSignature(fName_));
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin_->GetGPUVirtualAddress());

	//ライトの描画
	light_->Draw(cmdList, 4);

	// モデル描画
	model_->Draw(cmdList);
}

void FbxObject3d::LoadAnimation()
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//アニメーションカウント
	int sceneCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < sceneCount; i++)
	{
		//仮データ
		Animation tempData = {};
		//i番のアニメーション取得
		tempData.animstack = fbxScene->GetSrcObject<FbxAnimStack>(i);
		//アニメーションの名前を取得
		const char* animstackname = tempData.animstack->GetName();
		//アニメーションの時間情報
		tempData.takeinfo = fbxScene->GetTakeInfo(animstackname);
		//仮データを実データに入れる
		animationData_.push_back(tempData);
	}
}

void FbxObject3d::PlayAnimation(int animationNumber, bool isLoop)
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//アニメーションの変更
	fbxScene->SetCurrentAnimationStack(animationData_[animationNumber].animstack);
	//開始時間取得
	startTime_ = animationData_[animationNumber].takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime_ = animationData_[animationNumber].takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime_ = startTime_;
	//再生中状態にする
	isPlay_ = true;
	//ループさせるか
	isLoop_ = isLoop;

	if (animationEaseData_ != nullptr)
	{
		animationEaseData_->Reset();
	}
	interpolationEase->SetActFlag(true);
	interpolationEase->Reset();
	interpolationModel_ = nullptr;
}

XMFLOAT3 FbxObject3d::GetWorldPosition()
{
	XMFLOAT3 worldpos = {};
	worldpos.x = matWorld_.r[3].m128_f32[0];
	worldpos.y = matWorld_.r[3].m128_f32[1];
	worldpos.z = matWorld_.r[3].m128_f32[2];

	return worldpos;
}

const XMMATRIX FbxObject3d::GetBoneMatWorld(std::string name)
{
	//ボーン配列
	std::vector<FbxModel::Bone>& bone = model_->GetBones();
	XMMATRIX mat = {};

	for (int i = 0; i < bone.size(); i++)
	{
		if (bone[i].name == name)
		{
			//今の姿勢行列を取得
			FbxAMatrix fbxCurrentPose = bone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime_);
			//XMMATRIXに変換
			FbxLoader::ConvertMatrixFromFbx(&mat, fbxCurrentPose);
			//合成
			return model_->GetModelTransform() * mat;
		}
	}

	return {};
}

XMFLOAT3 FbxObject3d::ToXMFLOAT3(FbxVector4 count)
{
	XMFLOAT3 temp = {};
	temp.x = static_cast<float>(count[0]);
	temp.y = static_cast<float>(count[1]);
	temp.z = static_cast<float>(count[2]);

	return temp;
}

XMFLOAT4 FbxObject3d::ToXMFLOAT4(FbxQuaternion count)
{
	XMFLOAT4 temp = {};
	temp.x = static_cast<float>(count[0]);
	temp.y = static_cast<float>(count[1]);
	temp.z = static_cast<float>(count[2]);
	temp.w = static_cast<float>(count[3]);

	return temp;
}