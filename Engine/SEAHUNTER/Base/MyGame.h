#pragma once
#include "BaseGame.h"

class MyGame : public BaseGame
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
};