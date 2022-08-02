#include "UserInterface.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Initialize()
{
	lifeFrame_ = Sprite::Create(1, { 0,0 });
	strengthFrame_ = Sprite::Create(2, { 0,15 });
	lifeGauge_ = Sprite::Create(3, { 0,0 });
	strengthGauge_ = Sprite::Create(5, { 0,15 });
}

void UserInterface::Finalize()
{
}

void UserInterface::Update()
{
}

void UserInterface::BackDraw()
{
}

void UserInterface::NearDraw()
{
	strengthFrame_->Draw();
	lifeFrame_->Draw();
	strengthGauge_->Draw();
	lifeGauge_->Draw();
}
