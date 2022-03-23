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
		string mapName;
		std::vector<int> mapCsvNumber;
		int mapChipMaxX;
		int mapChipMaxY;
	};

private:
	MapChip();
	~MapChip();

public:
	MapChip(const MapChip& mapChip) = delete;
	MapChip& operator=(const MapChip& mapChip) = delete;

	static MapChip* GetInstance();

public:
	// ���[�g�p�X
	static const string baseDirectory;

public:
	//CSV�t�@�C���ǂݍ���
	void CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName);
	//�}�b�v�i���o�[�w��
	int GetChipNum(int x, int y, std::string fName, int mapChipSize = 1);
	//�傫���̍ő���擾
	const XMFLOAT2& GetMapChipMaxXY(std::string fName);

private:
	std::vector<MapChipData> mapData;
};