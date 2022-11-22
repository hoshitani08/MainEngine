#pragma once

#include <d3dx12.h>
#include <string>
#include <vector>

class ShaderManager
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::を省略
	using string = std::string;

public: // サブクラス
	enum class Type
	{
		Line,
		Triangle,
	};

	struct ShaderData
	{
		// シェーダファイル名
		std::wstring fName;
		//プリミティブ形状のタイプ
		Type type;
		//頂点シェーダオブジェクト
		ComPtr<ID3DBlob> vsBlob;
		//ピクセルシェーダオブジェクト
		ComPtr<ID3DBlob> psBlob;
		// ジオメトリシェーダオブジェクト
		ComPtr<ID3DBlob> gsBlob;
		//エラーオブジェクト
		ComPtr<ID3DBlob> errorBlob;
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

private:
	ShaderManager() = default;
	~ShaderManager() = default;

public:
	ShaderManager(const ShaderManager & r) = delete;
	ShaderManager& operator=(const ShaderManager & r) = delete;

	// 初期化
	void Initialize(ID3D12Device * device);
	// 終了
	void Finalize();
	// グラフィックパイプラインの生成
	void CreateGraphicsPipeline(ID3D12Device * device, std::wstring fName, std::string typeName, Type type = Type::Triangle);
	void FbxPipeline(ID3D12Device* device, ShaderData tempData);
	void ObjPipeline(ID3D12Device* device, ShaderData tempData);
	void PmxPipeline(ID3D12Device* device, ShaderData tempData);
	void SpritePipeline(ID3D12Device* device, ShaderData tempData);
	void ParticlePipeline(ID3D12Device* device, ShaderData tempData);
	void PostEffectPipeline(ID3D12Device* device, ShaderData tempData);

	ID3D12RootSignature* GetRootSignature(std::wstring fName, Type type = Type::Triangle);
	ID3D12PipelineState* GetPipelineState(std::wstring fName, Type type = Type::Triangle);

	// インスタンス生成
	static ShaderManager* GetInstance();

private:
	std::vector<ShaderData> shaderData_;
};