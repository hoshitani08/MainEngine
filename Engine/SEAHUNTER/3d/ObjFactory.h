#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Model.h"

class ObjFactory final
{
public: // �T�u�N���X
	struct ModelData
	{
		std::string fName; // �t�@�C����
		std::unique_ptr<Model> model; // OBJ���f��
	};

public:
	// ���[�g�p�X
	static const std::string BASE_DIRECTORY;

private:
	ObjFactory() = default;
	~ObjFactory() = default;

public:
	ObjFactory(const ObjFactory& objFactory) = delete;
	ObjFactory& operator=(const ObjFactory& objFactory) = delete;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	/// <param name="smoothing">�G�b�W�������t���O</param>
	void LoadModel(std::string fName, bool smoothing = false);
	/// <summary>
	/// ���f���̎擾
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	/// <returns>�����������f��</returns>
	Model* GetModel(std::string fName);
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static ObjFactory* GetInstance();

private:
	// ���f���f�[�^
	std::vector<ModelData> modelData_;
};