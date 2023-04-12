#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <memory>

// ���x���f�[�^
struct LevelData
{
	struct ObjectData
	{
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
	};
	struct ItemData
	{
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		int quantity;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
	std::vector<ItemData> items;
};

class LevelLoader
{
public: // �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public: // �����o�֐�
	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	static std::unique_ptr<LevelData> LoadFile(const std::string& fileName);
};

