#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"

namespace
{
	//�t�F�[�h�^�C�v
	enum class TYPE
	{
		FadeOut = -1,
		Not = 0,
		FadeIn = 1,
	};
}

class BlackBox
{
public: // �����o�֐�
	void Spin(TYPE type);
	void AddOffsetTimer(int num) { offsetTimer_ += num; }

public: // �����o�֐�
	int GetOffsetTimer() { return offsetTimer_; }

public: // �����o�ϐ�
	std::unique_ptr<Sprite> sprite_;
	int offsetTimer_ = 0;
	bool inEnd_ = false;
	bool outEnd_ = false;
};

class SceneChange
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �T�u�N���X
	

public: // �����o�֐�
	SceneChange();
	void Update();
	void Draw();
	void SceneChangeStart(const std::string& sceneName);

public: // �����o�֐�
	bool GetinEndFlag() { return inEndFlag_; }

private: // �����o�ϐ�
	bool inEndFlag_ = false;
	bool outEndFlag_ = false;

	std::vector<std::vector<std::unique_ptr<BlackBox>>> blackBox_;
	TYPE type_;
	std::string sceneName_;
};

