#include "MapChip.h"

using namespace DirectX;

// �ÓI�����o�ϐ��̎���
const std::string MapChip::baseDirectory = "Resources/csv/";

MapChip::MapChip()
{
}

MapChip::~MapChip()
{
}

MapChip* MapChip::GetInstance()
{
	static MapChip instance;
	return &instance;
}

void MapChip::CsvLoad(int mapChipMaxX, int mapChipMaxY, std::string fName)
{
	std::ifstream ifs(baseDirectory + fName + ".csv");
	std::string line;
	//���f�[�^�쐬
	MapChipData tempData;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;
		std::vector<int> result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(stoi(field));
		}
		for (auto i : result)
		{
			tempData.mapCsvNumber.push_back(i);
		}
	}
	//�t�@�C������ۑ�
	tempData.mapName = baseDirectory + fName + ".csv";
	//X�̍ő�l��ۑ�
	tempData.mapChipMaxX = mapChipMaxX;
	//Y�̍ő�l��ۑ�
	tempData.mapChipMaxY = mapChipMaxY;
	//�f�[�^�\���̂ɕۑ�
	mapData_.push_back(tempData);
}

int MapChip::GetChipNum(int x, int y, std::string fName, int mapChipSize)
{
	const int X = x / mapChipSize;
	const int Y = y / mapChipSize;
	int count = 0;

	for (int i = 0; i < mapData_.size(); i++)
	{
		if (mapData_[i].mapName == baseDirectory + fName + ".csv")
		{
			break;
		}
		count++;
	}

	if (X < 0 || X >= mapData_[count].mapChipMaxX || Y < 0 || Y >= mapData_[count].mapChipMaxY)
	{
		assert(0);
	}

	std::vector<int> map = mapData_[count].mapCsvNumber;

	return map[static_cast<std::vector<int, std::allocator<int>>::size_type>(Y) * mapData_[count].mapChipMaxX + X];
}

const int& MapChip::GetMapChipMaxX(std::string fName)
{
	int count = 0;

	for (int i = 0; i < mapData_.size(); i++)
	{
		if (mapData_[i].mapName == baseDirectory + fName + ".csv")
		{
			return mapData_[count].mapChipMaxX;
		}
		count++;
	}
	//�t�@�C�����Ȃ������ꍇ
	return 0;
}

const int& MapChip::GetMapChipMaxY(std::string fName)
{
	int count = 0;

	for (int i = 0; i < mapData_.size(); i++)
	{
		if (mapData_[i].mapName == baseDirectory + fName + ".csv")
		{
			return mapData_[count].mapChipMaxY;
		}
		count++;
	}
	//�t�@�C�����Ȃ������ꍇ
	return 0;
}
