#include "Camera.h"

using namespace DirectX;

Camera::Camera(int window_width, int window_height)
{
	aspectRatio_ = static_cast<float>(window_width) / static_cast<float>(window_height);

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection_ = matView_ * matProjection_;
}

void Camera::Update()
{
	if (viewDirty_ || projectionDirty_)
	{
		// 再計算必要なら
		if (viewDirty_)
		{
			// ビュー行列更新
			UpdateViewMatrix();
			viewDirty_ = false;
		}

		// 再計算必要なら
		if (projectionDirty_)
		{
			// ビュー行列更新
			UpdateProjectionMatrix();
			projectionDirty_ = false;
		}
		// ビュープロジェクションの合成
		matViewProjection_ = matView_ * matProjection_;
	}
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の更新
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye_);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target_);
	// (仮の)上方向
	XMVECTOR upVector = XMLoadFloat3(&up_);

	// カメラZ軸(視線方向)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラX軸(右方向)
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラY軸(上方向)
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラの回転行列
	XMMATRIX matCameraRot = DirectX::XMMatrixIdentity();
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転置により逆行列(逆回転)を計算
	matView_ = XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル(カメラの座標系)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); // X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); // Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); // Z成分
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	matView_.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard_.r[0] = cameraAxisX;
	matBillboard_.r[1] = cameraAxisY;
	matBillboard_.r[2] = cameraAxisZ;
	matBillboard_.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion 全方向ビルボード行列の計算

#pragma region Y軸回りビルボード行列の計算
	//カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//Y軸回りビルボード行列
	matBillboardY_.r[0] = ybillCameraAxisX;
	matBillboardY_.r[1] = ybillCameraAxisY;
	matBillboardY_.r[2] = ybillCameraAxisZ;
	matBillboardY_.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma endregion Y軸回りビルボード行列の計算
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影による射影行列の生成
	matProjection_ = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(60.0f),
		aspectRatio_,
		0.1f, 1000.0f
	);
}

void Camera::CameraMoveVector(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveEyeVector(const XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::CameraMoveTargetVector(const XMFLOAT3& move)
{
	XMFLOAT3 target_moved = GetTarget();

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetTarget(target_moved);
}