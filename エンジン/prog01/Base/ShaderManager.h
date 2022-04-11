#pragma once
#include <d3dx12.h>
#include <string>
#include <vector>

class ShaderManager
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::���ȗ�
	using string = std::string;

public: // �T�u�N���X
	struct ShaderData
	{
		// �V�F�[�_�t�@�C����
		std::wstring fName;
		//���_�V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> vsBlob;
		//�s�N�Z���V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> psBlob;
		//�G���[�I�u�W�F�N�g
		ComPtr<ID3DBlob> errorBlob;
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

private:
	ShaderManager() = default;
	~ShaderManager() = default;

public:
	ShaderManager(const ShaderManager & r) = delete;
	ShaderManager& operator=(const ShaderManager & r) = delete;

	// ������
	void Initialize(ID3D12Device * device);
	// �I��
	void Finalize();
	// �O���t�B�b�N�p�C�v���C���̐���
	void CreateGraphicsPipeline(ID3D12Device * device, std::wstring fName, std::string typeName);
	void FbxPipeline(ID3D12Device* device, ShaderData tempData);
	void ObjPipeline(ID3D12Device* device, ShaderData tempData);

	ID3D12RootSignature* GetRootSignature(std::wstring fName);
	ID3D12PipelineState* GetPipelineState(std::wstring fName);

	// �C���X�^���X����
	static ShaderManager* GetInstance();

private:
	std::vector<ShaderData> shaderData;
};