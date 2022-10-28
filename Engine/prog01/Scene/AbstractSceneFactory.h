#pragma once

#include <string>

#include "BaseScene.h"

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	//シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};