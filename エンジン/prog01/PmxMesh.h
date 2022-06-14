#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>

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

public: // メンバ関数

	//bool GetpTexture() { return pTexture != nullptr; }

private: // メンバ変数
	//unsigned vertexNum;
	//ID3D11ShaderResourceView* pTexture;
	//XMFLOAT4 diffuseColor;
	//XMFLOAT3 specularColor;
	//float specularity;
	//XMFLOAT3 ambientColor;

	//// Shader
	//ID3D11InputLayout* pInputLayout;
	//ID3D11VertexShader* pVertexShader;
	//ID3D11PixelShader* pPixelShader;
	//ComPtr<ID3D12Resource> pConstantBuffer;
	//D3D11_MAPPED_SUBRESOURCE resource;
	//ID3D11SamplerState* pSamplerState;
};