#pragma once
#include "BaseGame.h"

class MyGame : public BaseGame
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// I—¹
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update() override;
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;
};