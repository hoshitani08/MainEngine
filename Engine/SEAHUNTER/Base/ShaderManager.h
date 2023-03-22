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
	enum class Type
	{
		Line,
		Triangle,
	};

	struct ShaderData
	{
		// �V�F�[�_�t�@�C����
		std::wstring fName;
		//�v���~�e�B�u�`��̃^�C�v
		Type type;
		//���_�V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> vsBlob;
		//�s�N�Z���V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> psBlob;
		// �W�I���g���V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> gsBlob;
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

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void Initialize(ID3D12Device * device);
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="fName">�V�F�[�_�t�@�C����</param>
	/// <param name="typeName">�^�C�v</param>
	/// <param name="type">�v���~�e�B�u�`��̃^�C�v</param>
	void CreateGraphicsPipeline(ID3D12Device * device, const std::wstring& fName, const std::string& typeName, Type type = Type::Triangle);
	void FbxPipeline(ID3D12Device* device, ShaderData tempData);
	void ObjPipeline(ID3D12Device* device, ShaderData tempData);
	void PmxPipeline(ID3D12Device* device, ShaderData tempData);
	void SpritePipeline(ID3D12Device* device, ShaderData tempData);
	void ParticlePipeline(ID3D12Device* device, ShaderData tempData);
	void PostEffectPipeline(ID3D12Device* device, ShaderData tempData);
	/// <summary>
	/// ���[�g�V�O�l�`���̎擾
	/// </summary>
	/// <param name="fName">�V�F�[�_�t�@�C����</param>
	/// <param name="type">�v���~�e�B�u�`��̃^�C�v</param>
	/// <returns>���[�g�V�O�l�`��</returns>
	ID3D12RootSignature* GetRootSignature(const std::wstring& fName, Type type = Type::Triangle);
	/// <summary>
	/// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <param name="fName">�V�F�[�_�t�@�C����</param>
	/// <param name="type">�v���~�e�B�u�`��̃^�C�v</param>
	/// <returns>�p�C�v���C���X�e�[�g�I�u�W�F�N�g</returns>
	ID3D12PipelineState* GetPipelineState(const std::wstring& fName, Type type = Type::Triangle);
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static ShaderManager* GetInstance();

private:
	std::vector<ShaderData> shaderData_;
};