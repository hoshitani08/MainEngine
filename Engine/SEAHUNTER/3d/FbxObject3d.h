#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "LightGroup.h"

class FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// 定数
	//ボーンの最大数
	static const int MAX_BONES = 200;

public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列用）
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

	// 定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	// アニメーション用データ構造体
	struct Animation
	{
		FbxAnimStack* animstack;
		FbxTakeInfo* takeinfo;
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);
	/// <summary>
	/// 静的破棄
	/// </summary>
	static void StaticFinalize();
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="HLSLfName">HLSL名</param>
	/// <param name="isAnimation">アニメーションフラグ</param>
	/// <returns></returns>
	static std::unique_ptr<FbxObject3d> Create(FbxModel* model = nullptr, std::wstring HLSLfName = L"BasicFBX", bool isAnimation = false);
	/// <summary>
	/// デバイスの設定
	/// </summary>
	/// <param name="device">デバイス</param>
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device_ = device; }
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) { FbxObject3d::camera_ = camera; }
	/// <summary>
	/// ライトの設定
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(LightGroup* light) { FbxObject3d::light_ = light; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// カメラ
	static Camera* camera_;
	//ライト
	static LightGroup* light_;

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FbxObject3d();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// モデルを設定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* model) { model_ = model; }
	/// <summary>
	/// アニメーションのロード
	/// </summary>
	void LoadAnimation();
	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="animationNumber">アニメーションハンドル</param>
	/// <param name="isLoop">ループさせるか</param>
	void PlayAnimation(int animationNumber = 0, bool isLoop = true);
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position_; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	/// <summary>
	/// X,Y,Z軸回りの取得
	/// </summary>
	/// <returns>X,Y,Z軸回りの角度</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }
	/// <summary>
	/// X,Y,Z軸回りの設定
	/// </summary>
	/// <param name="rotation">X,Y,Z軸回り角度</param>
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns>スケール</returns>
	const XMFLOAT3& GetScale() { return scale_; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }
	/// <summary>
	/// ボーン行列の取得
	/// </summary>
	/// <param name="name">ボーン名</param>
	/// <returns>ボーン行列</returns>
	const XMMATRIX GetBoneMatWorld(std::string name);
	/// <summary>
	/// アニメーションが終わったか
	/// </summary>
	/// <returns></returns>
	bool AnimationEnd() { return currentTime_ >= endTime_; }

protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin_;
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_ = {};
	// モデル
	FbxModel* model_ = nullptr;
	//1フレームの時間
	FbxTime frameTime_ = {};
	//アニメーション開始時間
	FbxTime startTime_ = {};
	//アニメーション終了時間
	FbxTime endTime_ = {};
	//現在時間(アニメーション)
	FbxTime currentTime_ = {};
	//アニメーション再生中
	bool isPlay_ = false;
	//ループフラグ
	bool isLoop_ = false;
	//アニメーションのデータ
	std::vector<Animation> animationData_ = {};
	// シェーダファイル名
	std::wstring fName_ = L"";
};