#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>

#include "PmxMaterial.h"

using namespace DirectX;

class PmxMesh
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
		// 追加UV座標
		std::vector<XMFLOAT4> additionalUV;

		// ボーンウェイト
		struct Weight
		{
			enum Type
			{
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
			};

			Type type;
			int born1;
			int	born2;
			int	born3;
			int	born4;
			float weight1;
			float weight2;
			float weight3;
			float weight4;
			XMFLOAT3 c;
			XMFLOAT3 r0;
			XMFLOAT3 r1;
		} weight;

		// エッジ倍率
		float edgeMagnif;
	};

	struct Bone
	{
		// ボーン名
		std::wstring name;
		// English version
		std::string nameEnglish;
		XMFLOAT3 position;
		int parentIndex;
		int transformationLevel;
		unsigned short flag;
		XMFLOAT3 coordOffset;
		int childrenIndex;
		int impartParentIndex;
		float impartRate;
		// 固定軸方向ベクトル
		XMFLOAT3 fixedAxis;
		// ローカルのX軸方向ベクトル
		XMFLOAT3 localAxisX;
		// ローカルのZ軸方向ベクトル
		XMFLOAT3 localAxisZ;
		int externalParentKey;
		int ikTargetIndex;
		int ikLoopCount;
		float ikUnitAngle;
		struct IKLink
		{
			int index;
			bool existAngleLimited;
			XMFLOAT3 limitAngleMin;
			XMFLOAT3 limitAngleMax;
		};
		std::vector<IKLink> ikLinks;
	};

	struct Surface
	{
		int vertexIndex;
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

public: // メンバ関数
	~PmxMesh();
	// 名前を取得
	const std::string& GetName() { return name_; }
	// 名前をセット
	void SetName(const std::string& name) { name_ = name; }
	// 頂点データの追加
	void AddVertex(const VertexPosNormalUv& vertex) { vertices_.emplace_back(vertex); }
	// 頂点インデックスの追加
	void AddIndex(unsigned short index) { indices_.emplace_back(index); }
	// 頂点データの数を取得
	inline size_t GetVertexCount() { return vertices_.size(); }
	// マテリアルの取得
	PmxMaterial* GetMaterial() { return material_; }
	// マテリアルの割り当て
	void SetMaterial(PmxMaterial* material) { material_ = material; }
	// バッファの生成
	void CreateBuffers();
	// 頂点バッファ取得
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }
	// インデックスバッファ取得
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// 頂点配列を取得
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }
	// インデックス配列を取得
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

private: // メンバ変数
	// 名前
	std::string name_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// マテリアル
	PmxMaterial* material_ = nullptr;

public:
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	std::vector<Surface> surfaces;
	// 骨データ配列
	std::vector<Bone> bones;
};