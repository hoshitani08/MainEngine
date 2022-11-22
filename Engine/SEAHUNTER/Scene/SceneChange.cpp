#include "SceneChange.h"
#include "SceneManager.h"

SceneChange::SceneChange()
{
	type_ = TYPE::FadeIn;
	
	for (int x = 0; x < 32; x++)
	{
		std::vector<std::unique_ptr<BlackBox>> tempX;
		for (int y = 0; y < 18; y++)
		{
			std::unique_ptr<BlackBox> tempY = std::make_unique<BlackBox>();

			tempY->sprite_ = Sprite::Create(103, { (float)(20 + x * 40),(float)(20 + y * 40) }, { 1,1,1,1 }, { 0.5f,0.5f });
			tempY->offsetTimer_ = (31 - x + y);

			tempX.push_back(std::move(tempY));
		}
		blackBox_.push_back(std::move(tempX));
	}
}

void SceneChange::Update()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			if (blackBox_[x][y]->GetOffsetTimer() <= 0)
			{
				blackBox_[x][y]->Spin(type_);
			}
			else
			{
				blackBox_[x][y]->AddOffsetTimer(-1);
			}

		}
	}

	//末端がtrueならそれを全体に適応
	if (blackBox_[0][17]->inEnd_)
	{
		inEndFlag_ = true;
	}
	if (blackBox_[0][17]->outEnd_)
	{
		outEndFlag_ = true;
		SceneManager::GetInstance()->ChangeScene(sceneName_);
	}
}

void SceneChange::Draw()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			blackBox_[x][y]->sprite_->Draw();
		}
	}
}

void SceneChange::SceneChangeStart(const std::string& sceneName)
{
	if (type_ != TYPE::FadeOut)
	{
		outEndFlag_ = false;

		for (int x = 0; x < blackBox_.size(); x++)
		{
			for (int y = 0; y < blackBox_[x].size(); y++)
			{
				blackBox_[x][y]->sprite_->SetRotation(0.0f);
				blackBox_[x][y]->sprite_->SetScale(0.0f);
				blackBox_[x][y]->offsetTimer_ = (31 - x + y);
			}
		}
	}
	type_ = TYPE::FadeOut;

	sceneName_ = sceneName;
}

void BlackBox::Spin(TYPE type)
{
	//終了していれば処理を省略
	if (type == TYPE::FadeIn && inEnd_ || type == TYPE::FadeOut && outEnd_)
	{
		return;
	}

	//回転
	float rotate = sprite_->GetRotation();
	rotate += 3;
	sprite_->SetRotation(rotate);

	//縮小
	float scale = sprite_->GetScale();
	scale -= 0.05f * (int)type;
	//見えなくなったら終了
	if (type == TYPE::FadeIn && scale <= 0.0f)
	{
		scale = 0.0f;
		inEnd_ = true;
	}
	else if (type == TYPE::FadeOut && scale >= 1.0f && (int)rotate % 90 == 0)
	{
		scale = 1.0f;
		outEnd_ = true;
	}

	sprite_->SetScale(scale);
}
