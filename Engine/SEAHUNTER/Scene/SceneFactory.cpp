#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "LoadScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;
	
	if (sceneName == "TitleScene")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GameScene")
	{
		newScene = new GameScene();
	}
	else if (sceneName == "ClearScene")
	{
		newScene = new ClearScene();
	}
	else if (sceneName == "GameOverScene")
	{
		newScene = new GameOverScene();
	}
	else if (sceneName == "LoadScene")
	{
		newScene = new LoadScene();
	}

	return newScene;
}
