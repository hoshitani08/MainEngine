#include "DirectXCommon.h"

#include <algorithm>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

DirectXCommon::DirectXCommon()
{

}

DirectXCommon::~DirectXCommon()
{
	// Direct3D関連
	dxgiFactory_.Reset();
	commandList_.Reset();
	commandAllocator_.Reset();
	commandQueue_.Reset();
	for (auto& a : backBuffers_)
	{
		a.Reset();
	}
	swapChain_.Reset();
	depthBuffer_.Reset();
	rtvHeaps_.Reset();
	dsvHeap_.Reset();
	fence_.Reset();
	device_.Reset();
}

void DirectXCommon::Initialize(WinApp* winApp)
{
	// nullptrチェック
	assert(winApp);

	winApp_ = winApp;

	reference_ = std::chrono::steady_clock::now();

	// DXGIデバイス初期化
	if (!InitializeDXGIDevice())
	{
		assert(0);
	}

	// コマンド関連初期化
	if (!InitializeCommand())
	{
		assert(0);
	}

	// スワップチェーンの生成
	if (!CreateSwapChain())
	{
		assert(0);
	}

	// レンダーターゲット生成
	if (!CreateFinalRenderTargets())
	{
		assert(0);
	}

	// 深度バッファ生成
	if (!CreateDepthBuffer())
	{
		assert(0);
	}

	// フェンス生成
	if (!CreateFence())
	{
		assert(0);
	}
}

void DirectXCommon::PreDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), bbIndex, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// 全画面クリア
	ClearRenderTarget();
	// 深度バッファクリア
	ClearDepthBuffer();

	// ビューポートの設定
	commandList_->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT));
	// シザリング矩形の設定
	commandList_->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT));
}

void DirectXCommon::PostDraw()
{
	// リソースバリアを変更（描画対象→表示状態）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
	commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	commandList_->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandList_.Get() }; // コマンドリストの配列
	commandQueue_->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	swapChain_->Present(1, 0);

	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// max 60fps 固定
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 60ギリギリだとちょっとばかし高いリフレッシュレートのモニタで逆にかくついてしまうので少しバッファを取る
	static const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 62.0f));
	// 実際にwaitするのは60基準
	static const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	std::chrono::microseconds check = kMinCheckTime - elapsed;
	if (std::chrono::microseconds(0) < check)
	{
		std::chrono::microseconds waitTime = kMinTime - elapsed;

		// sleepは信用ならないので1uでポーリング
		std::chrono::steady_clock::time_point waitStart = std::chrono::steady_clock::now();
		do
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		} while (std::chrono::steady_clock::now() - waitStart < waitTime);
	}

	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - reference_);
	reference_ = std::chrono::steady_clock::now();

	commandAllocator_->Reset(); // キューをクリア
	commandList_->Reset(commandAllocator_.Get(), nullptr); // 再びコマンドリストを貯める準備
}

void DirectXCommon::ClearRenderTarget()
{
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), bbIndex, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
	commandList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;
	return &instance;
}

bool DirectXCommon::InitializeDXGIDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0;
		dxgiFactory_->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description; // アダプター名

		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // 採用
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		// デバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device_));
		if (SUCCEEDED(result))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> d3dInfoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&d3dInfoQueue))))
	{
		// Suppress individual messages by their ID.
		D3D12_MESSAGE_ID denyIds[] =
		{
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,
		};

		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		d3dInfoQueue->AddStorageFilterEntries(&filter);
	}
#endif

	return true;
}

bool DirectXCommon::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::WINDOW_WIDTH;
	swapchainDesc.Height = WinApp::WINDOW_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式を一般的なものに
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファとして使えるように
	swapchainDesc.BufferCount = 2; // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フルスクリーン切り替えを許可
	//IDXGISwapChain1のComPtrを用意
	ComPtr<IDXGISwapChain1> swapchain1;
	HWND hwnd = winApp_->GetHwnd();
	//スワップチェーンを生成
	result = dxgiFactory_->CreateSwapChainForHwnd
	(
		commandQueue_.Get(),
		winApp_->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}
	swapchain1.As(&swapChain_);

	return true;
}

bool DirectXCommon::InitializeCommand()
{
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// コマンドリストを生成
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	return true;
}

bool DirectXCommon::CreateFinalRenderTargets()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapChain_->GetDesc(&swcDesc);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 裏表の２つ分について
	backBuffers_.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers_.size(); i++)
	{
		// スワップチェーンからバッファを取得
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), i, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView
		(
			backBuffers_[i].Get(),
			nullptr,
			handle
		);
	}

	return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::WINDOW_WIDTH,
		WinApp::WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	// リソースの生成
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer_)
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView
	(
		depthBuffer_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

bool DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}