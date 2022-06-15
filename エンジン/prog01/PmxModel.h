#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>

#include "PmxMesh.h"

using namespace DirectX;

// PMXのモデルデータ型
struct PMXModelData
{
	static constexpr int NO_DATA_FLAG = -1;

	struct Vertex
	{
		// 頂点座標
		XMFLOAT3 position;
		// 法線
		XMFLOAT3 normal;
		// UV座標
		XMFLOAT2 uv;
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

	struct Surface
	{
		int vertexIndex;
	};

	struct Material
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 specular;
		float specularity;
		XMFLOAT3 ambient;

		int colorMapTextureIndex;
		int toonTextureIndex;
		// (スフィアテクスチャは非対応)

		// 材質ごとの頂点数
		int vertexNum;
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


	std::vector<Vertex> vertices;
	std::vector<Surface> surfaces;
	std::vector<std::wstring> texturePaths;
	std::vector<Material> materials;
	std::vector<Bone> bones;
};

class PmxModel
{
public:
	//フレンドクラス
	friend class PmxLoader;

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
	// texturedModelシェーダーの定数バッファデータ型
	struct TexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// notTexturedModelシェーダーの定数バッファデータ型
	struct NotTexturedModelConstantBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMVECTOR diffuseColor;
		XMVECTOR ambientColor;
	};

public:
	inline size_t getMeshesSize() { return meshes.size(); }
	//inline bool meshHasTexture(int _meshIndex) { return meshes[_meshIndex]->GetpTexture(); }
	bool Initialize(const std::wstring& _filePath, ID3D12Device* const _pDevice);
	//void Draw(DirectX11& _directX11, int _meshIndex, const void* const _pConstantBufferData);

private:
	enum VertexBuffer
	{
		POSITION,
		UV,
		NORMAL,
		VertexBuffer_SIZE,
	};

	//std::array<ComPtr<ID3D12Resource>, VertexBuffer_SIZE> pVertexBuffers;
	ComPtr<ID3D12Resource> pIndexBuffer;
	unsigned mIndexCount;

	PMXModelData allData;
	std::vector<PmxMesh*> meshes;

	HRESULT createTexturedShader(ID3D12Device* const _pDevice, PmxMesh& mesh);
	HRESULT createNotTexturedShader(ID3D12Device* const _pDevice, PmxMesh& mesh);
};