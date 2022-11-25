#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

// ノード
struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

//テクスチャデータ
struct TextureData
{
	//テクスチャメタデータ
	DirectX::TexMetadata metaData = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//SRVのGPUハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:// 定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;
	//テクスチャの最大数
	static const int MAX_TEXTURES = 4;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;              //xyz座標
		DirectX::XMFLOAT3 normal;           //法線ベクトル
		DirectX::XMFLOAT2 uv;               //uv座標
		DirectX::XMFLOAT3 tangent;          //接ベクトル
		DirectX::XMFLOAT3 binormal;         //従ベクトル
		UINT boneIndex[MAX_BONE_INDICES];   //ボーン番号
		float boneWeight[MAX_BONE_INDICES]; //ボーン重み
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

public:
	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void CreateBuffers(ID3D12Device* device);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// モデルの変形行列取得
	/// </summary>
	/// <returns>変形行列</returns>
	const XMMATRIX& GetModelTransform() { return meshNode_->globalTransform; }
	/// <summary>
	/// テクスチャバッファ生成
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="device">デバイス</param>
	/// <param name="srvIndex"></param>
	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);
	/// <summary>
	/// ボーンの取得
	/// </summary>
	/// <returns>ボーン</returns>
	std::vector<Bone>& GetBones() { return bones_; }
	/// <summary>
	/// FBXシーンの取得デストラクタ
	/// </summary>
	/// <returns>FBXシーン</returns>
	FbxScene* GetFbxScene() { return fbxScene_; }
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FbxModel();

private:
	//モデル名
	std::string name_;
	//ノード配列
	std::vector<Node> nodes_;
	//ボーン配列
	std::vector<Bone> bones_;
	//FBXシーン
	FbxScene* fbxScene_ = nullptr;
	//メッシュを持つノード
	Node* meshNode_ = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices_;
	//頂点インデックス配列
	std::vector<unsigned short> indices_;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metaData_ = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg_ = {};
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
	//ベーステクスチャ
	TextureData baseTexture_;
	//メタルネステクスチャ
	TextureData metalnessTexture_;
	//法線テクスチャ
	TextureData normalTexture_;
};