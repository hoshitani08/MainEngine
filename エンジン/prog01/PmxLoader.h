#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

#include "PmxModel.h"

using namespace DirectX;

class PmxLoader
{
private: // �G�C���A�X
	// std::���ȗ�
	using string = std::string;
	using wstring = std::wstring;
	using ifstream = std::ifstream;

public: // �萔
	// ���f���i�[���[�g�p�X
	static const wstring baseDirectory;

public:
	// �V���O���g���C���X�^���X�̎擾
	static PmxLoader* GetInstance();
	// PMX���f���f�[�^�̃��[�h�֐�
	PmxModel* loadPMX(const wstring& _filePath);

private:
	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	PmxLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~PmxLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	PmxLoader(const PmxLoader& obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const PmxLoader& obj) = delete;
	bool GetPMXStringUTF16(ifstream& _file, wstring& output);
};