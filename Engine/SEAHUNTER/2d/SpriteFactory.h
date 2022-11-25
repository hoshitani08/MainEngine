#pragma once

#include "Sprite.h"
#include "DebugText.h"

class SpriteFactory final
{
private:
	SpriteFactory() = default;
	~SpriteFactory() = default;

public:
	SpriteFactory(const SpriteFactory& spriteFactory) = delete;
	SpriteFactory& operator=(const SpriteFactory& spriteFactory) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SpriteFactory* GetInstance();
};