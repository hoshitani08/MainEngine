#pragma once

#include <string>

#include "BaseScene.h"

class AbstractSceneFactory
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AbstractSceneFactory() = default;
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns></returns>
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};