#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include <chrono>

#include "WinApp.h"

class DirectXCommon final
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win"></param>
	void Initialize(WinApp* win);
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();
	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();
	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();
	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }
	/// <summary>
	/// 描画コマンドリストの取得
	/// </summary>
	/// <returns>描画コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:
	DirectXCommon();
	~DirectXCommon();

public:
	DirectXCommon(const DirectXCommon& directXCommon) = delete;
	DirectXCommon& operator=(const DirectXCommon& directXCommon) = delete;
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
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
	ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	HANDLE frameLatencyWaitableObject_;
	std::chrono::steady_clock::time_point reference_;

private: // メンバ関数
	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	/// <returns></returns>
	bool InitializeDXGIDevice();
	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	/// <returns></returns>
	bool CreateSwapChain();
	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	/// <returns></returns>
	bool InitializeCommand();
	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	/// <returns></returns>
	bool CreateFinalRenderTargets();
	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	/// <returns></returns>
	bool CreateDepthBuffer();
	/// <summary>
	/// フェンス生成
	/// </summary>
	/// <returns></returns>
	bool CreateFence();
};