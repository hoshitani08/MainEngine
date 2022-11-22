#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"

namespace
{
	//フェードタイプ
	enum class TYPE
	{
		FadeOut = -1,
		Not = 0,
		FadeIn = 1,
	};
}

class BlackBox
{
public: // メンバ関数
	void Spin(TYPE type);
	void AddOffsetTimer(int num) { offsetTimer_ += num; }

public: // メンバ関数
	int GetOffsetTimer() { return offsetTimer_; }

public: // メンバ変数
	std::unique_ptr<Sprite> sprite_;
	int offsetTimer_ = 0;
	bool inEnd_ = false;
	bool outEnd_ = false;
};

class SceneChange
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // サブクラス
	

public: // メンバ関数
	SceneChange();
	void Update();
	void Draw();
	void SceneChangeStart(const std::string& sceneName);

public: // メンバ関数
	bool GetinEndFlag() { return inEndFlag_; }

private: // メンバ変数
	bool inEndFlag_ = false;
	bool outEndFlag_ = false;

	std::vector<std::vector<std::unique_ptr<BlackBox>>> blackBox_;
	TYPE type_;
	std::string sceneName_;
};

