#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "WinApp.h"

class DirectXCommon final
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	// 初期化
	void Initialize(WinApp* win);
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
	// レンダーターゲットのクリア
	void ClearRenderTarget();
	// 深度バッファのクリア
	void ClearDepthBuffer();
	// デバイスの取得
	ID3D12Device* GetDevice() { return device_.Get(); }
	// 描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:
	DirectXCommon();
	~DirectXCommon();

public:
	DirectXCommon(const DirectXCommon& directXCommon) = delete;
	DirectXCommon& operator=(const DirectXCommon& directXCommon) = delete;

	static DirectXCommon* GetInstance();

private: // メンバ変数
	// ウィンドウズアプリケーション管理
	WinApp* winApp_ = nullptr;

	// Direct3D関連
	ComPtr<IDXGIFactory6> dxgiFactory_;
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<IDXGISwapChain4> swapchain_;
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

private: // メンバ関数
	// DXGIデバイス初期化
	bool InitializeDXGIDevice();
	// スワップチェーンの生成
	bool CreateSwapChain();
	// コマンド関連初期化
	bool InitializeCommand();
	// レンダーターゲット生成
	bool CreateFinalRenderTargets();
	// 深度バッファ生成
	bool CreateDepthBuffer();
	// フェンス生成
	bool CreateFence();
};