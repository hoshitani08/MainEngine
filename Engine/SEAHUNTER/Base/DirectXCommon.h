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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="win"></param>
	void Initialize(WinApp* win);
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();
	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();
	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃N���A
	/// </summary>
	void ClearRenderTarget();
	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
	void ClearDepthBuffer();
	/// <summary>
	/// �f�o�C�X�̎擾
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }
	/// <summary>
	/// �`��R�}���h���X�g�̎擾
	/// </summary>
	/// <returns>�`��R�}���h���X�g</returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:
	DirectXCommon();
	~DirectXCommon();

public:
	DirectXCommon(const DirectXCommon& directXCommon) = delete;
	DirectXCommon& operator=(const DirectXCommon& directXCommon) = delete;
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
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
	ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	HANDLE frameLatencyWaitableObject_;
	std::chrono::steady_clock::time_point reference_;

private: // �����o�֐�
	/// <summary>
	/// DXGI�f�o�C�X������
	/// </summary>
	/// <returns></returns>
	bool InitializeDXGIDevice();
	/// <summary>
	/// �X���b�v�`�F�[���̐���
	/// </summary>
	/// <returns></returns>
	bool CreateSwapChain();
	/// <summary>
	/// �R�}���h�֘A������
	/// </summary>
	/// <returns></returns>
	bool InitializeCommand();
	/// <summary>
	/// �����_�[�^�[�Q�b�g����
	/// </summary>
	/// <returns></returns>
	bool CreateFinalRenderTargets();
	/// <summary>
	/// �[�x�o�b�t�@����
	/// </summary>
	/// <returns></returns>
	bool CreateDepthBuffer();
	/// <summary>
	/// �t�F���X����
	/// </summary>
	/// <returns></returns>
	bool CreateFence();
};