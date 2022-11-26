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
	// Direct3D�֘A
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
	// nullptr�`�F�b�N
	assert(winApp);

	winApp_ = winApp;

	reference_ = std::chrono::steady_clock::now();

	// DXGI�f�o�C�X������
	if (!InitializeDXGIDevice())
	{
		assert(0);
	}

	// �R�}���h�֘A������
	if (!InitializeCommand())
	{
		assert(0);
	}

	// �X���b�v�`�F�[���̐���
	if (!CreateSwapChain())
	{
		assert(0);
	}

	// �����_�[�^�[�Q�b�g����
	if (!CreateFinalRenderTargets())
	{
		assert(0);
	}

	// �[�x�o�b�t�@����
	if (!CreateDepthBuffer())
	{
		assert(0);
	}

	// �t�F���X����
	if (!CreateFence())
	{
		assert(0);
	}
}

void DirectXCommon::PreDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// ���\�[�X�o���A��ύX�i�\����ԁ��`��Ώہj
	commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), bbIndex, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// �����_�[�^�[�Q�b�g���Z�b�g
	commandList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �S��ʃN���A
	ClearRenderTarget();
	// �[�x�o�b�t�@�N���A
	ClearDepthBuffer();

	// �r���[�|�[�g�̐ݒ�
	commandList_->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT));
	// �V�U�����O��`�̐ݒ�
	commandList_->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::WINDOW_WIDTH, WinApp::WINDOW_HEIGHT));
}

void DirectXCommon::PostDraw()
{
	// ���\�[�X�o���A��ύX�i�`��Ώہ��\����ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
	commandList_->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// ���߂̃N���[�Y
	commandList_->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { commandList_.Get() }; // �R�}���h���X�g�̔z��
	commandQueue_->ExecuteCommandLists(1, cmdLists);

	// �o�b�t�@���t���b�v
	swapChain_->Present(1, 0);

	// �R�}���h���X�g�̎��s������҂�
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// max 60fps �Œ�
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 60�M���M�����Ƃ�����Ƃ΂����������t���b�V�����[�g�̃��j�^�ŋt�ɂ������Ă��܂��̂ŏ����o�b�t�@�����
	static const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 62.0f));
	// ���ۂ�wait����̂�60�
	static const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	std::chrono::microseconds check = kMinCheckTime - elapsed;
	if (std::chrono::microseconds(0) < check)
	{
		std::chrono::microseconds waitTime = kMinTime - elapsed;

		// sleep�͐M�p�Ȃ�Ȃ��̂�1u�Ń|�[�����O
		std::chrono::steady_clock::time_point waitStart = std::chrono::steady_clock::now();
		do
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		} while (std::chrono::steady_clock::now() - waitStart < waitTime);
	}

	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - reference_);
	reference_ = std::chrono::steady_clock::now();

	commandAllocator_->Reset(); // �L���[���N���A
	commandList_->Reset(commandAllocator_.Get(), nullptr); // �ĂуR�}���h���X�g�𒙂߂鏀��
}

void DirectXCommon::ClearRenderTarget()
{
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), bbIndex, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// �S��ʃN���A        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F
	commandList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer()
{
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
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
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0;
		dxgiFactory_->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // ���I�z��ɒǉ�����
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); // �A�_�v�^�[�̏����擾

		// �\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description; // �A�_�v�^�[��

		// Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // �̗p
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		// �f�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device_));
		if (SUCCEEDED(result))
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
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

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::WINDOW_WIDTH;
	swapchainDesc.Height = WinApp::WINDOW_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏�������ʓI�Ȃ��̂�
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�Ƃ��Ďg����悤��
	swapchainDesc.BufferCount = 2; // �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͑��₩�ɔj��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �t���X�N���[���؂�ւ�������
	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapchain1;
	HWND hwnd = winApp_->GetHwnd();
	//�X���b�v�`�F�[���𐶐�
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

	// �R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �R�}���h���X�g�𐶐�
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �W���ݒ�ŃR�}���h�L���[�𐶐�
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

	// �e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// ���\�̂Q���ɂ���
	backBuffers_.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers_.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), i, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// �����_�[�^�[�Q�b�g�r���[�̐���
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

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::WINDOW_WIDTH,
		WinApp::WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	// ���\�[�X�̐���
	result = device_->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer_)
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
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

	// �t�F���X�̐���
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}