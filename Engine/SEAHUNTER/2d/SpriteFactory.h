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
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static SpriteFactory* GetInstance();
};