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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	// ������
	void Initialize(WinApp* win);
	// �`��O����
	void PreDraw();
	// �`��㏈��
	void PostDraw();
	// �����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget();
	// �[�x�o�b�t�@�̃N���A
	void ClearDepthBuffer();
	// �f�o�C�X�̎擾
	ID3D12Device* GetDevice() { return device_.Get(); }
	// �`��R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:
	DirectXCommon();
	~DirectXCommon();

public:
	DirectXCommon(const DirectXCommon& directXCommon) = delete;
	DirectXCommon& operator=(const DirectXCommon& directXCommon) = delete;

	static DirectXCommon* GetInstance();

private: // �����o�ϐ�
	// �E�B���h�E�Y�A�v���P�[�V�����Ǘ�
	WinApp* winApp_ = nullptr;

	// Direct3D�֘A
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

private: // �����o�֐�
	// DXGI�f�o�C�X������
	bool InitializeDXGIDevice();
	// �X���b�v�`�F�[���̐���
	bool CreateSwapChain();
	// �R�}���h�֘A������
	bool InitializeCommand();
	// �����_�[�^�[�Q�b�g����
	bool CreateFinalRenderTargets();
	// �[�x�o�b�t�@����
	bool CreateDepthBuffer();
	// �t�F���X����
	bool CreateFence();
};