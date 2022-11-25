#pragma once

#include <string>

#include "BaseScene.h"

class AbstractSceneFactory
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~AbstractSceneFactory() = default;
	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns></returns>
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};