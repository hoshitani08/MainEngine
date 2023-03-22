#include "Stage.h"
#include "ObjFactory.h"
#include "MapChip.h"
#include "DirectXCommon.h"

#include <cassert>
#include <sstream>
#include <iomanip>

Stage::Stage(Monster* monster, Hunter* hunter, Camera* camera, const std::string& fName)
{
	hunter_ = hunter;
	monster_ = monster;
	camera_ = camera;
	Initialize(fName);
}

void Stage::Initialize(const std::string& fName)
{
	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile(fName);

	for (auto& objectData : levelData->objects)
	{
		bool flag = false;

		if (objectData.fileName == "Rock" || objectData.fileName == "Rock2" || objectData.fileName == "coral")
		{
			flag = true;
		}

		if (flag)
		{
			std::unique_ptr<Block> block;

			// 座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);

			if (objectData.fileName == "Rock")
			{
				block = std::make_unique<Block>(0, pos);

				block_.push_back(std::move(block));
			}
			else if (objectData.fileName == "coral")
			{
				block = std::make_unique<Block>(1, pos);

				block_.push_back(std::move(block));
			}
			else if (objectData.fileName == "Rock2")
			{
				block = std::make_unique<Block>(2, pos);

				block_.push_back(std::move(block));
			}
		}
		else
		{
			// ファイル名から登録済みモデルを検索
			Model* model = ObjFactory::GetInstance()->GetModel(objectData.fileName);
			if (model == nullptr)
			{
				model = ObjFactory::GetInstance()->GetModel("cube");
			}

			// モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3d> newObject = Object3d::Create(model);

			// 座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);

			// 回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation(rot);

			// サイズ
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);

			if (objectData.fileName == "cube")
			{
				newObject->SetPrimitiveType(ShaderManager::Type::Line);
			}

			// 配列に登録
			objects.push_back(std::move(newObject));
		}
	}

	fugitiveBustParticle = ParticleManager::Create(DirectXCommon::GetInstance()->GetDevice(), camera_, L"sandEffect");
	fugitiveBustEmitter = std::make_unique<ParticleEmitter>(fugitiveBustParticle.get());
	fugitiveBustEmitter->SetCenter(1);
	XMFLOAT4 color = { 0.1f, 0.1f, 0.1f, 1.0f };
	fugitiveBustEmitter->SetStartColor(color);
	fugitiveBustEmitter->SetEndColor(color);
	fugitiveBustEmitter->SetVelocity(0.03f);
}

void Stage::Finalize()
{
}

void Stage::Update()
{
	SandParticle();

	fugitiveBustEmitter->Update();
	for (auto& object : objects)
	{
		object->Update();
	}
	for (auto& a : block_)
	{
		a->Update();
	}
}

void Stage::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (auto& object : objects)
	{
		object->Draw(cmdList);
	}
	for (auto& a : block_)
	{
		a->Draw(cmdList);
	}
	
	fugitiveBustEmitter->Draw(cmdList);
}

void Stage::SandParticle()
{
	if (hunter_ == nullptr)
	{
		return;
	}

	if (hunter_->GetPosition().y <= 1.0f && hunter_->IsMoveFlag())
	{
		XMFLOAT3 pos = hunter_->GetPosition();
		pos.y -= 1.0f;
		fugitiveBustEmitter->SandAdd(8, 120, pos, ObjFactory::GetInstance()->GetModel("sand"));
	}
}
