#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

class MapChip final
{
private: // �G�C���A�X
	// std::���ȗ�
	using string = std::string;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;

public: // �T�u�N���X
	// �A�j���[�V�����p�f�[�^�\����
	struct MapChipData
	{
		string mapName = ""; // .csv��
		std::vector<int> mapCsvNumber = {}; // �i�[�R���e�i
		int mapChipMaxX = 0; // �ő�̉���
		int mapChipMaxY = 0; // �ő�̏c��
	};

private:
	MapChip();
	~MapChip();

public:
	MapChip(const MapChip& mapChip) = delete;
	MapChip& operator=(const MapChip& mapChip) = delete;
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static MapChip* GetInstance();

public:
	// ���[�g�p�X
	static const string baseDirectory;

public:
	/// <summary>
	/// CSV�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="mapChipMaxX">�ő�̉���</param>
	/// <param name="mapChipMaxY">�ő�̏c��</param>
	/// <param name="fName">�t�@�C����</param>
	void CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName);
	/// <summary>
	/// �}�b�v�i���o�[�w��
	/// </summary>
	/// <param name="x">��</param>
	/// <param name="y">�c</param>
	/// <param name="fName">�t�@�C����</param>
	/// <param name="mapChipSize"></param>
	/// <returns></returns>
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);
	/// <summary>
	/// �ő�̉������擾
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	/// <returns>�ő�̉���</returns>
	const int& GetMapChipMaxX(std::string fName);
	/// <summary>
	/// �ő�̏c�����擾
	/// </summary>
	/// <param name="fName">�t�@�C����</param>
	/// <returns>�ő�̏c��</returns>
	const int& GetMapChipMaxY(std::string fName);

private:
	std::vector<MapChipData> mapData_;
};