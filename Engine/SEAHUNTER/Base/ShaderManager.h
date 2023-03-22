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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device * device);
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="fName">シェーダファイル名</param>
	/// <param name="typeName">タイプ</param>
	/// <param name="type">プリミティブ形状のタイプ</param>
	void CreateGraphicsPipeline(ID3D12Device * device, const std::wstring& fName, const std::string& typeName, Type type = Type::Triangle);
	void FbxPipeline(ID3D12Device* device, ShaderData tempData);
	void ObjPipeline(ID3D12Device* device, ShaderData tempData);
	void PmxPipeline(ID3D12Device* device, ShaderData tempData);
	void SpritePipeline(ID3D12Device* device, ShaderData tempData);
	void ParticlePipeline(ID3D12Device* device, ShaderData tempData);
	void PostEffectPipeline(ID3D12Device* device, ShaderData tempData);
	/// <summary>
	/// ルートシグネチャの取得
	/// </summary>
	/// <param name="fName">シェーダファイル名</param>
	/// <param name="type">プリミティブ形状のタイプ</param>
	/// <returns>ルートシグネチャ</returns>
	ID3D12RootSignature* GetRootSignature(const std::wstring& fName, Type type = Type::Triangle);
	/// <summary>
	/// パイプラインステートオブジェクトの取得
	/// </summary>
	/// <param name="fName">シェーダファイル名</param>
	/// <param name="type">プリミティブ形状のタイプ</param>
	/// <returns>パイプラインステートオブジェクト</returns>
	ID3D12PipelineState* GetPipelineState(const std::wstring& fName, Type type = Type::Triangle);
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ShaderManager* GetInstance();

private:
	std::vector<ShaderData> shaderData_;
};