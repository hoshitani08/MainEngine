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
		std::string fName;
		std::unique_ptr<Model> model;
	};

public:
	// ���[�g�p�X
	static const std::string baseDirectory;

private:
	ObjFactory() = default;
	~ObjFactory() = default;

public:
	ObjFactory(const ObjFactory& objFactory) = delete;
	ObjFactory& operator=(const ObjFactory& objFactory) = delete;

	// ������
	void Initialize();
	// �I��
	void Finalize();
	// ���f���̃��[�h
	void LoadModel(std::string fName, bool smoothing = false);
	// ���f���̎擾
	Model* GetModel(std::string fName);
	// �C���X�^���X����
	static ObjFactory* GetInstance();

private:
	std::vector<ModelData> modelData;
};