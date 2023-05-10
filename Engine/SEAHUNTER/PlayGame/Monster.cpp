#include "Monster.h"
#include "FbxFactory.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "DebugText.h"
#include "ObjFactory.h"
#include "ItemManager.h"
#include "Ease.h"
#include "BaseCalculate.h"
#include "EnemyBullet.h"

#include <math.h>
#include <algorithm>

std::unique_ptr<Monster> Monster::Create(Camera* camera, Hunter* hunter)
{
	// 3Dオブジェクトのインスタンスを生成
	Monster* monster = new Monster();
	if (monster == nullptr)
	{
		return nullptr;
	}
	monster->SetHunter(hunter);
	monster->Initialize(camera);

	return std::unique_ptr<Monster>(monster);
}

void Monster::Initialize(Camera* camera)
{
	nucleus_ = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
	nucleus_->SetPosition({ 0.0f, 10.0f, 20.0f });

	for (int i = 0; i < body_.size(); i++)
	{
		body_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			body_[i]->SetParent(nucleus_.get());
		}
		else
		{
			body_[i]->SetPosition({ -1.5f, 0.0f, 0.0f });
			body_[i]->SetParent(body_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 5Ui64>::size_type>(i) - 1].get());
		}

		body_[i]->SetColor({ 0.4f, 0.8f, 0.4f, 1.0f });
	}

	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightForeFoot_[i]->SetPosition({ 0.0f, 0.0f, -1.0f });
			rightForeFoot_[i]->SetParent(body_[1].get());
		}
		else if (i == 1)
		{
			rightForeFoot_[i]->SetPosition({ 0.0f, -1.0f, -1.0f });
			rightForeFoot_[i]->SetParent(rightForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			rightForeFoot_[i]->SetPosition({ 1.0f, -1.0f, 0.0f });
			rightForeFoot_[i]->SetParent(rightForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		rightForeFoot_[i]->SetColor({ 1.0f, 0.5f, 0.25f, 1.0f });
	}

	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftForeFoot_[i]->SetPosition({ 0.0f, 0.0f, 1.0f });
			leftForeFoot_[i]->SetParent(body_[1].get());
		}
		else if (i == 1)
		{
			leftForeFoot_[i]->SetPosition({ 0.0f, -1.0f, 1.0f });
			leftForeFoot_[i]->SetParent(leftForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			leftForeFoot_[i]->SetPosition({ 1.0f, -1.0f, 0.0f });
			leftForeFoot_[i]->SetParent(leftForeFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		leftForeFoot_[i]->SetColor({ 1.0f, 0.5f, 0.25f, 1.0f });
	}

	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			rightHindFoot_[i]->SetPosition({ -5.5f, 0.0f, -1.0f });
			rightHindFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			rightHindFoot_[i]->SetPosition({ 0.0f, -1.0f, -1.0f });
			rightHindFoot_[i]->SetParent(rightHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			rightHindFoot_[i]->SetPosition({ -1.0f, -1.0f, 0.0f });
			rightHindFoot_[i]->SetParent(rightHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		rightHindFoot_[i]->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
	}

	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			leftHindFoot_[i]->SetPosition({ -5.5f, 0.0f, 1.0f });
			leftHindFoot_[i]->SetParent(body_[0].get());
		}
		else if (i == 1)
		{
			leftHindFoot_[i]->SetPosition({ 0.0f, -1.0f, 1.0f });
			leftHindFoot_[i]->SetParent(leftHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}
		else
		{
			leftHindFoot_[i]->SetPosition({ -1.0f, -1.0f, 0.0f });
			leftHindFoot_[i]->SetParent(leftHindFoot_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 3Ui64>::size_type>(i) - 1].get());
		}

		leftHindFoot_[i]->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
	}

	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i] = Object3d::Create(ObjFactory::GetInstance()->GetModel("sphere"));
		if (i == 0)
		{
			tail_[i]->SetPosition({ -1.5f, 0.0f, 0.0f });
			tail_[i]->SetParent(body_[4].get());
		}
		else
		{
			tail_[i]->SetPosition({ -1.5f, 0.0f, 0.0f });
			tail_[i]->SetParent(tail_[static_cast<std::array<std::unique_ptr<Object3d, std::default_delete<Object3d>>, 4Ui64>::size_type>(i) - 1].get());
		}

		tail_[i]->SetColor({ 0.5f, 0.5f, 0.0f, 1.0f });
	}

	bloodParticle_ = std::make_unique<ObjParticle>();
	bloodEmitter_ = std::make_unique<ParticleEmitter>(bloodParticle_.get());
	bloodEmitter_->SetCenter(1);
	float scale = 0.4f;
	bloodEmitter_->SetObjStartScale({ scale, scale, scale });
	bloodEmitter_->SetStartColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	bloodEmitter_->SetEndColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	bubbleParticle_ = std::make_unique<ObjParticle>();
	bubbleEmitter_ = std::make_unique<ParticleEmitter>(bubbleParticle_.get());
	scale = 0.1f;
	bubbleEmitter_->SetCenter(1);
	bubbleEmitter_->SetObjStartScale({ scale, scale, scale });
	bubbleEmitter_->SetObjEndScale({ scale, scale, scale });
	bubbleEmitter_->SetStartColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	bubbleEmitter_->SetEndColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	//ビヘイビアツリーの初期化
	behaviorTree_ = std::make_unique<BehaviorTree>();

	behaviorTree_->AddNodeFunc([this]() { return Howl(); }, BehaviorTree::CreateType::End);

	behaviorTree_->SetNodeFuncType(BehaviorTreeNode::NodeType::Sequence);
	behaviorTree_->AddNodeFunc([this]() { return AttackElapsedTime(); }, BehaviorTree::CreateType::Add);
	behaviorTree_->AddNodeFunc([this]() { return AttackModeSelection(); }, BehaviorTree::CreateType::Add);
	behaviorTree_->AddNodeFunc([this]() { return AttackModeMove(); }, BehaviorTree::CreateType::End);

	behaviorTree_->SetNodeFuncType(BehaviorTreeNode::NodeType::Sequence);
	behaviorTree_->AddNodeFunc([this]() { return WaitingElapsedTime(); }, BehaviorTree::CreateType::Add);
	behaviorTree_->AddNodeFunc([this]() { return WaitingModeSelection(); }, BehaviorTree::CreateType::Add);
	behaviorTree_->AddNodeFunc([this]() { return WaitingModeMove(); }, BehaviorTree::CreateType::End);

	behaviorTree_->AddNodeFunc([this]() { return Dead(); }, BehaviorTree::CreateType::End);

	attackSelector_.push_back([this]() { return AttackMode1(); });
	attackSelector_.push_back([this]() { return AttackMode2(); });
	attackSelector_.push_back([this]() { return AttackMode3(); });
	attackSelector_.push_back([this]() { return AttackMode4(); });
	attackSelector_.push_back([this]() { return AttackMode5(); });
	attackSelector_.push_back([this]() { return AttackMode6(); });

	waitingSelector_.push_back([this]() { return WaitingMode1(); });
	waitingSelector_.push_back([this]() { return WaitingMode2(); });
	waitingSelector_.push_back([this]() { return WaitingMode3(); });

	TreeReset();

	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };
	XMFLOAT3 enemyRot = nucleus_->GetRotation();

	enemyRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (ANGLE / PI);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(result.y - 0.0f, result.x - 0.0f) * (ANGLE / PI);

	nucleus_->SetRotation(enemyRot);

	for (auto& a : tailMoveChangeFlag_)
	{
		a = true;
	}
	for (auto& a : tailMoveResetFlag_)
	{
		a = true;
	}

	// 段階の構築
	animationFunc_.push_back([this]() { return InitialPosture(); });
	animationFunc_.push_back([this]() { return Move(); });
	animationFunc_.push_back([this]() { return Assault(); });
	animationFunc_.push_back([this]() { return TailAttack(); });
	animationFunc_.push_back([this]() { return RightPunch(); });
	animationFunc_.push_back([this]() { return LeftPunch(); });
	animationFunc_.push_back([this]() { return Waiting(); });
	animationFunc_.push_back([this]() { return Tornado(); });
}

void Monster::Finalize()
{
}

void Monster::Update()
{
	XMFLOAT3 pos = nucleus_->GetPosition();

	// 当たり判定
	pos.x = std::clamp(pos.x, -48.0f, 48.0f);
	pos.y = std::clamp(pos.y, 1.0f, 58.0f);
	pos.z = std::clamp(pos.z, -48.0f, 48.0f);

	XMFLOAT3 pos1 = rightForeFoot_[2]->GetWorldPosition();

	if (pos1.y <= 1.0f)
	{
		pos.y -= pos1.y - 1.0f;
	}

	pos1 = leftForeFoot_[2]->GetWorldPosition();

	if (pos1.y <= 1.0f)
	{
		pos.y -= pos1.y - 1.0f;
	}

	pos1 = rightHindFoot_[2]->GetWorldPosition();

	if (pos1.y <= 1.0f)
	{
		pos.y -= pos1.y - 1.0f;
	}

	pos1 = leftHindFoot_[2]->GetWorldPosition();

	if (pos1.y <= 1.0f)
	{
		pos.y -= pos1.y - 1.0f;
	}

	nucleus_->SetPosition(pos);

	int count = 0;
	for (auto& bullet : bullet_)
	{
		if (bullet->Hit(hunter_->GetPosition()))
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10.0f);
		}

		if (bullet->IsEndFlag())
		{
			bullet_.erase(bullet_.begin() + count);
		}
		count++;
	}

	//更新
	nucleus_->Update();
	for (int i = 0; i < body_.size(); i++)
	{
		body_[i]->Update();
	}
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i]->Update();
	}
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i]->Update();
	}
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i]->Update();
	}
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i]->Update();
	}
	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i]->Update();
	}
	for (auto& bullet : bullet_)
	{
		bullet->Update();
	}
	bloodEmitter_->Update();
	bubbleEmitter_->Update();
}

void Monster::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (int i = 0; i < body_.size(); i++)
	{
		body_[i]->Draw(cmdList);
	}
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i]->Draw(cmdList);
	}
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i]->Draw(cmdList);
	}
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i]->Draw(cmdList);
	}
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i]->Draw(cmdList);
	}
	for (int i = 0; i < tail_.size(); i++)
	{
		tail_[i]->Draw(cmdList);
	}
	for (auto& bullet : bullet_)
	{
		bullet->Draw(cmdList);
	}
	bloodEmitter_->Draw(cmdList);
	bubbleEmitter_->Draw(cmdList);
}

void Monster::AllMove()
{
	behaviorTree_->Run();

	if (colorTimer_ >= 30)
	{
		for (int i = 0; i < body_.size(); i++)
		{
			body_[i]->SetColor({ 0.4f, 0.8f, 0.4f, 1.0f });
		}

		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			rightForeFoot_[i]->SetColor({ 1.0f, 0.5f, 0.25f, 1.0f });
		}

		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			leftForeFoot_[i]->SetColor({ 1.0f, 0.5f, 0.25f, 1.0f });
		}

		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			rightHindFoot_[i]->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
		}

		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			leftHindFoot_[i]->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
		}

		for (int i = 0; i < tail_.size(); i++)
		{
			tail_[i]->SetColor({ 0.5f, 0.5f, 0.0f, 1.0f });
		}
		colorTimer_ = 0;
		colorChangeFlag_ = false;
	}

	if (hunter_->GetActFlag())
	{
		colorTimer_ = 0;
		damageHitFlag_ = false;
		hunter_->SetActFalg(false);
	}

	if (colorChangeFlag_)
	{
		colorTimer_++;
	}

	PartsTailDestruction();
	TailBullet();
}

void Monster::AngleAdjustment()
{
	XMFLOAT3 vector = { hunter_->GetPosition().x - nucleus_->GetPosition().x, hunter_->GetPosition().y - nucleus_->GetPosition().y, hunter_->GetPosition().z - nucleus_->GetPosition().z };
	XMFLOAT3 enemyRot = nucleus_->GetRotation();

	enemyRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (ANGLE / PI);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-enemyRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	enemyRot.z = atan2(result.y - 0.0f, result.x - 0.0f) * (ANGLE / PI);

	float timeRate = 0.0f;
	int countNum = 30;
	timeRate = angleEaseTimer_ / countNum;
	angleEaseTimer_++;

	nucleus_->SetRotation(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, nucleus_->GetRotation(), enemyRot, timeRate));

	if (angleEaseTimer_ > countNum)
	{
		nucleus_->SetRotation(enemyRot);
	}
}

bool Monster::Hit(XMFLOAT3 partsPosition, float enemyRange, float playerRange)
{
	Sphere eSphere;
	Sphere pSphere;

	eSphere.center = { partsPosition.x, partsPosition.y, partsPosition.z, 1.0f };
	eSphere.radius = enemyRange;
	pSphere.center = { hunter_->GetPosition().x, hunter_->GetPosition().y, hunter_->GetPosition().z, 1.0f };
	pSphere.radius = playerRange;

	if (Collision::CheckSphere2Sphere(eSphere, pSphere))
	{
		return true;
	}
	return false;
}

void Monster::DamageHit(Sphere hitSphere)
{
	Sphere eSphere;

	int count = 15;
	int life = 60;
	XMFLOAT4 color = { 7.0f, 6.0f, 0.0f, 1.0f };

	// 体
	for (int i = 0; i < body_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::Body), Parts::Body, hitSphere, body_[i].get());
	}

	// 右前足
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::RightForeFoot), Parts::RightForeFoot, hitSphere, rightForeFoot_[i].get());
	}

	// 左前足
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::LeftForeFoot), Parts::LeftForeFoot, hitSphere, leftForeFoot_[i].get());
	}

	// 右後足
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::RightHindFoot), Parts::RightHindFoot, hitSphere, rightHindFoot_[i].get());
	}

	// 左後足
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::LeftHindFoot), Parts::LeftHindFoot, hitSphere, leftHindFoot_[i].get());
	}

	// 尻尾
	for (int i = 0; i < tail_.size(); i++)
	{
		if (!hunter_->IsAttackFlag())
		{
			break;
		}
		DamageCalculate(static_cast<float>(PartsDamage::Tail), Parts::Tail, hitSphere, tail_[i].get());
	}
}

void Monster::PartsTailDestruction()
{
	if (tailDestruction_ >= 150 && !tailDestructionFlag_)
	{
		XMFLOAT3 ppos = tail_[0]->GetWorldPosition();

		tail_[0]->SetParent((Object3d*)nullptr);

		XMFLOAT3 prot = tail_[0]->GetRotation();

		prot.x += nucleus_->GetRotation().x;
		prot.y += nucleus_->GetRotation().y;
		prot.z += nucleus_->GetRotation().z;

		tail_[0]->SetPosition(ppos);
		tail_[0]->SetRotation(prot);

		for (int i = 1; i < tail_.size(); i++)
		{
			XMFLOAT3 ppos = tail_[i]->GetWorldPosition();

			tail_[i]->SetParent((Object3d*)nullptr);

			tail_[i]->SetPosition(ppos);
		}

		tailDestructionFlag_ = true;
	}
}

bool Monster::Howl()
{
	if (howlflag_)
	{
		return false;
	}
	else
	{
		animationFunc_[static_cast<int>(AnimationType::Waiting)]();
		howlTimer_++;
	}

	if (howlTimer_ >= 90)
	{
		howlflag_ = true;
	}

	return true;
}

bool Monster::Dead()
{
	if (hp_ >= 1.0f)
	{
		TreeReset();
	}
	else
	{
		isDead_ = true;
	}

	return true;
}

bool Monster::AttackElapsedTime()
{
	if (attackEnd_)
	{
		return false;
	}

	if (attackElapsedTimer_ >= 60 && (body_[0]->GetRotation().x <= 0.0f && attackSelect_[0]) ||
		attackSelect_[1] && isEaseFlag_ || attackSelect_[2] && isEaseFlag_ || attackSelect_[3] && isEaseFlag_ ||
		attackSelect_[4] && isEaseFlag_ || attackSelect_[5] && isEaseFlag_)
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		hitFlag_ = false;
		attackEnd_ = true;
		return false;
	}

	if (trackingEnd_)
	{
		attackElapsedTimer_++;
	}

	return true;
}

bool Monster::AttackModeSelection()
{
	for (auto& flag : attackSelect_)
	{
		if (flag)
		{
			return true;
		}
	}

	if (!Hit(body_[2]->GetWorldPosition(), 1.0f, 50.0f))
	{
		int count = static_cast<int>(RandCalculate(0.0f, 2.0f));

		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;

		if (count < 1.0f)
		{
			attackSelect_[0] = true;
			return true;
		}
		else
		{
			attackSelect_[5] = true;
			return true;
		}
		return true;
	}
	else if (Hit(body_[2]->GetWorldPosition(), 15.0f, 1.0f) && !Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		int count = static_cast<int>(RandCalculate(0.0f, 2.0f));

		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;

		if (count < 1.0f)
		{
			attackSelect_[1] = true;
			return true;
		}
		else
		{
			attackSelect_[4] = true;
			return true;
		}
	}
	else if (Hit(rightForeFoot_[2]->GetWorldPosition(), 4.5f, 1.0f))
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		attackSelect_[2] = true;
		return true;
	}
	else if (Hit(leftForeFoot_[2]->GetWorldPosition(), 4.5f, 1.0f))
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		attackSelect_[3] = true;
		return true;
	}

	attackEnd_ = true;
	return false;
}

bool Monster::AttackModeMove()
{
	for (int i = 0; i < attackSelector_.size(); i++)
	{
		if (attackSelect_[i])
		{
			return attackSelector_[i]();
		}
	}

	return false;
}

bool Monster::AttackMode1()
{
	animationFunc_[static_cast<int>(AnimationType::Assault)]();

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 1.0f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	if (!trackingEnd_)
	{
		AngleAdjustment();
		saveVector_ = vector;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 20.0f))
	{
		trackingEnd_ = true;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
	{
		hunter_->SetDamageFlag(true);
		hunter_->SetDamage(10.0f);
		hitFlag_ = true;
	}

	pos.x += saveVector_.x;
	pos.y += saveVector_.y;
	pos.z += saveVector_.z;

	nucleus_->SetPosition(pos);

	return true;
}

bool Monster::AttackMode2()
{
	animationFunc_[static_cast<int>(AnimationType::TailAttack)]();

	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	for (int i = 0; i < tail_.size(); i++)
	{
		if (hitFlag_)
		{
			continue;
		}
		if (Hit(tail_[i]->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10.0f);
			hitFlag_ = true;
		}
	}

	return true;
}

bool Monster::AttackMode3()
{
	animationFunc_[static_cast<int>(AnimationType::RightPunch)]();

	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		if (hitFlag_)
		{
			continue;
		}
		if (Hit(rightForeFoot_[i]->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10.0f);
			hitFlag_ = true;
		}
	}

	return true;
}

bool Monster::AttackMode4()
{
	animationFunc_[static_cast<int>(AnimationType::LeftPunch)]();

	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		if (hitFlag_)
		{
			continue;
		}
		if (Hit(leftForeFoot_[i]->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10.0f);
			hitFlag_ = true;
		}
	}

	return true;
}

bool Monster::AttackMode5()
{
	animationFunc_[static_cast<int>(AnimationType::Tornado)]();

	if (!trackingEnd_)
	{
		trackingEnd_ = true;
	}

	if (tornadoTimer_ >= 300)
	{
		isEaseFlag_ = true;
	}

	tornadoTimer_++;

	attackRange_ += 0.1f;

	attackRange_ = max(attackRange_, 10.0f);

	if (Hit(body_[3]->GetWorldPosition(), attackRange_, 1.0f) && hunter_->GetInvincibleTimer() >= 60 && !hitFlag_)
	{
		hunter_->SetDamageFlag(true);
		hunter_->SetDamage(10.0f);
		hitFlag_ = true;
	}

	XMFLOAT3 pos = hunter_->GetPosition();
	XMFLOAT3 vector = { pos.x - nucleus_->GetPosition().x, pos.y - nucleus_->GetPosition().y, pos.z - nucleus_->GetPosition().z };

	float speed = 0.1f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x -= vector.x;
	pos.y -= vector.y;
	pos.z -= vector.z;

	hunter_->SetPosition(pos);

	return true;
}

bool Monster::AttackMode6()
{
	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 1.0f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	if (!trackingEnd_)
	{
		AngleAdjustment();
		trackingEnd_ = true;
	}

	if (!bulletFlag_)
	{
		std::unique_ptr<EnemyBullet> temp = EnemyBullet::Create(body_[0]->GetWorldPosition(), nucleus_->GetRotation(), vector);

		bullet_.push_back(std::move(temp));
		bulletFlag_ = true;
		isEaseFlag_ = true;
	}

	return true;
}

bool Monster::WaitingElapsedTime()
{
	if (waitingElapsedTimer_ >= 60 || waitingEnd_)
	{
		if (count_ >= maxCount_)
		{
			count_ = 0;
			maxCount_ = static_cast<int>(RandCalculate(1.0f, 4.0f));
		}
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		return false;
	}

	waitingElapsedTimer_++;

	return true;
}

bool Monster::WaitingModeSelection()
{
	if (waitingSelect_[0] || waitingSelect_[1] || waitingSelect_[2])
	{
		return true;
	}

	if (Hit(nucleus_->GetPosition(), 1.0f, 1.0f) || count_ >= maxCount_)
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		waitingSelect_[0] = true;
		return true;
	}
	else if (!Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		waitingSelect_[1] = true;
		return true;
	}
	else if (Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		animationFunc_[static_cast<int>(AnimationType::InitialPosture)]();
		angleEaseTimer_ = 0.0f;
		waitingSelect_[2] = true;
		return true;
	}

	return false;
}

bool Monster::WaitingModeMove()
{
	for (int i = 0; i < waitingSelector_.size(); i++)
	{
		if (waitingSelect_[i])
		{
			return waitingSelector_[i]();
		}
	}

	return false;
}

bool Monster::WaitingMode1()
{
	animationFunc_[static_cast<int>(AnimationType::Waiting)]();
	return true;
}

bool Monster::WaitingMode2()
{
	animationFunc_[static_cast<int>(AnimationType::Move)]();
	AngleAdjustment();

	int countNum = 30;
	if (angleEaseTimer_ > countNum)
	{
		waitingEnd_ = true;
		return false;
	}

	if (Hit(body_[2]->GetWorldPosition(), 10.0f, 1.0f))
	{
		return true;
	}

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = 0.8f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	nucleus_->SetPosition(pos);

	return true;
}

bool Monster::WaitingMode3()
{
	if (!Hit(body_[2]->GetWorldPosition(), 80.0f, 1.0f))
	{
		waitingEnd_ = true;
		return false;
	}

	animationFunc_[static_cast<int>(AnimationType::Move)]();
	AngleAdjustment();

	XMFLOAT3 pos = nucleus_->GetPosition();
	XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

	float speed = -1.0f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	pos.x += vector.x;
	pos.y += vector.y;
	pos.z += vector.z;

	nucleus_->SetPosition(pos);

	return true;
}

void Monster::TreeReset()
{
	attackSelect_.clear();
	for (int i = 0; i < attackSelector_.size(); i++)
	{
		bool flag = false;
		attackSelect_.push_back(flag);
	}

	waitingSelect_.clear();
	for (int i = 0; i < waitingSelector_.size(); i++)
	{
		bool flag = false;
		waitingSelect_.push_back(flag);
	}

	isEaseFlag_ = false;
	easeTimer_ = 0.0f;
	attackElapsedTimer_ = 0;
	waitingElapsedTimer_ = 0;
	attackRange_ = 1.0f;
	tornadoTimer_ = 0;
	bulletFlag_ = false;

	trackingEnd_ = false;
	attackEnd_ = false;
	waitingEnd_ = false;

	count_++;
}

void Monster::DamageCalculate(float partsDamage, Parts parts, Sphere hitSphere, Object3d* temp)
{
	int count = 15;
	int life = 60;
	XMFLOAT4 color = { 7.0f, 6.0f, 0.0f, 1.0f };
	Sphere eSphere = { temp->GetWorldPosition().x, temp->GetWorldPosition().y, temp->GetWorldPosition().z, 1.0f };

	if (Collision::CheckSphere2Sphere(eSphere, hitSphere) && !damageHitFlag_)
	{
		damageHitFlag_ = true;
		colorChangeFlag_ = true;
		if (parts == Parts::Tail && !tailDestructionFlag_)
		{
			tailDestruction_ += 10;
			bloodEmitter_->BloodAdd(count, life, temp->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));
			hp_ -= partsDamage * ItemManager::GetInstance()->AttackBuffMagnification();
		}
		else if (parts != Parts::Tail)
		{
			bloodEmitter_->BloodAdd(count, life, temp->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("blood"));
			hp_ -= partsDamage * ItemManager::GetInstance()->AttackBuffMagnification();
		}

		for (int i = 0; i < body_.size(); i++)
		{
			if (parts != Parts::Body)
			{
				break;
			}
			body_[i]->SetColor(color);
		}
		for (int i = 0; i < rightForeFoot_.size(); i++)
		{
			if (parts != Parts::RightForeFoot)
			{
				break;
			}
			rightForeFoot_[i]->SetColor(color);
		}
		for (int i = 0; i < leftForeFoot_.size(); i++)
		{
			if (parts != Parts::LeftForeFoot)
			{
				break;
			}
			leftForeFoot_[i]->SetColor(color);
		}
		for (int i = 0; i < rightHindFoot_.size(); i++)
		{
			if (parts != Parts::RightHindFoot)
			{
				break;
			}
			rightHindFoot_[i]->SetColor(color);
		}
		for (int i = 0; i < leftHindFoot_.size(); i++)
		{
			if (parts != Parts::LeftHindFoot)
			{
				break;
			}
			leftHindFoot_[i]->SetColor(color);
		}
		for (int i = 0; i < tail_.size(); i++)
		{
			if (parts != Parts::Tail || tailDestructionFlag_)
			{
				break;
			}
			tail_[i]->SetColor(color);
		}
	}
}

void Monster::TailBullet()
{
	if (!tailDestructionFlag_)
	{
		return;
	}

	tailMoveTimer_++;

	if (tailMoveTimer_ <= 120)
	{
		return;
	}

	int num = 0;
	int count = 5;
	int life = 60;
	for (auto& a : tail_)
	{
		if ((a->GetPosition().x <= -48.0f || a->GetPosition().x >= 48.0f ||
			a->GetPosition().y <=   1.0f || a->GetPosition().y >= 58.0f ||
			a->GetPosition().z <= -48.0f || a->GetPosition().z >= 48.0f) && !tailMoveResetFlag_[num])
		{
			if (!tailMoveChangeFlag_[num])
			{
				tailMoveChangeFlag_[num] = true;
			}
			else
			{
				tailMoveChangeFlag_[num] = false;
			}
			
			tailMoveResetFlag_[num] = true;
		}

		XMFLOAT3 rot = a->GetRotation();
		XMFLOAT3 pos = a->GetPosition();

		if (tailMoveChangeFlag_[num])
		{
			if (tailMoveResetFlag_[num])
			{
				rot.x = RandCalculate(0.0f, 360.0f);
				rot.y = RandCalculate(0.0f, 360.0f);
				rot.z = RandCalculate(0.0f, 360.0f);

				a->SetRotation(rot);
				tailMoveResetFlag_[num] = false;
			}
			
			pos.x += cosf((rot.x * 3.14f) / 180.0f) * 0.5f;
			pos.y += sinf((rot.y * 3.14f) / 180.0f) * 0.5f;
			pos.z += -sinf((rot.x * 3.14f) / 180.0f) * 0.5f;
		}
		else
		{
			if (tailMoveResetFlag_[num])
			{
				XMFLOAT3 pos = a->GetPosition();
				XMFLOAT3 vector = { hunter_->GetPosition().x - pos.x, hunter_->GetPosition().y - pos.y, hunter_->GetPosition().z - pos.z };

				float speed = 0.5f;
				float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
				saveTailMoveVector_ = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

				tailMoveResetFlag_[num] = false;
			}

			pos.x += saveTailMoveVector_.x;
			pos.y += saveTailMoveVector_.y;
			pos.z += saveTailMoveVector_.z;
		}

		pos.x = std::clamp(pos.x, -48.0f, 48.0f);
		pos.y = std::clamp(pos.y, 1.0f, 58.0f);
		pos.z = std::clamp(pos.z, -48.0f, 48.0f);

		a->SetPosition(pos);

		if (Hit(a->GetWorldPosition(), 1.0f, 1.0f) && hunter_->GetInvincibleTimer() >= 60)
		{
			hunter_->SetDamageFlag(true);
			hunter_->SetDamage(10.0f);
		}

		bubbleEmitter_->BubbleAdd(count, life, a->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));

		num++;
	}
}

void Monster::InitialPosture()
{
	XMFLOAT3 rot = {};

	for (int i = 0; i < body_.size(); i++)
	{
		body_[i]->SetRotation(rot);
	}
	for (int i = 0; i < rightForeFoot_.size(); i++)
	{
		rightForeFoot_[i]->SetRotation(rot);
	}
	for (int i = 0; i < leftForeFoot_.size(); i++)
	{
		leftForeFoot_[i]->SetRotation(rot);
	}
	for (int i = 0; i < rightHindFoot_.size(); i++)
	{
		rightHindFoot_[i]->SetRotation(rot);
	}
	for (int i = 0; i < leftHindFoot_.size(); i++)
	{
		leftHindFoot_[i]->SetRotation(rot);
	}
	for (int i = 0; i < tail_.size(); i++)
	{
		if (!tailDestructionFlag_)
		{
			tail_[i]->SetRotation(rot);
		}
	}

	for (int i = 0; i < body_.size(); i++)
	{
		if (i == 0)
		{
			body_[i]->SetPosition({});
		}
		else
		{
			body_[i]->SetPosition({ -1.5f, 0.0f, 0.0f });
		}
	}
	rightForeFoot_[0]->SetPosition({ 0.0f, 0.0f, -1.0f });
	rightForeFoot_[1]->SetPosition({ 0.0f, -1.0f, -1.0f });
	rightForeFoot_[2]->SetPosition({ 1.0f, -1.0f, 0.0f });

	leftForeFoot_[0]->SetPosition({ 0.0f, 0.0f, 1.0f });
	leftForeFoot_[1]->SetPosition({ 0.0f, -1.0f, 1.0f });
	leftForeFoot_[2]->SetPosition({ 1.0f, -1.0f, 0.0f });

	rightHindFoot_[0]->SetPosition({ -5.5f, 0.0f, -1.0f });
	rightHindFoot_[1]->SetPosition({ 0.0f, -1.0f, -1.0f });
	rightHindFoot_[2]->SetPosition({ -1.0f, -1.0f, 0.0f });

	leftHindFoot_[0]->SetPosition({ -5.5f, 0.0f, 1.0f });
	leftHindFoot_[1]->SetPosition({ 0.0f, -1.0f, 1.0f });
	leftHindFoot_[2]->SetPosition({ -1.0f, -1.0f, 0.0f });

	for (int i = 0; i < tail_.size(); i++)
	{
		if (!tailDestructionFlag_)
		{
			tail_[i]->SetPosition({ -1.5f, 0.0f, 0.0f });
		}
	}
}

void Monster::Move()
{
	// 幅の制限
	float restrictionAngle = 135.0f;

	XMFLOAT3 rotA = rightForeFoot_[0]->GetRotation();
	rotA.z -= addAngle_;
	rightForeFoot_[0]->SetRotation(rotA);
	leftHindFoot_[0]->SetRotation(rotA);

	XMFLOAT3 rotB = leftForeFoot_[0]->GetRotation();
	rotB.z += addAngle_;
	leftForeFoot_[0]->SetRotation(rotB);
	rightHindFoot_[0]->SetRotation(rotB);

	if (rotB.z > 45.0f)
	{
		addAngle_ = -5.0f;
	}
	else if (rotB.z < -45.0f)
	{
		addAngle_ = 5.0f;
	}

	for (auto& a : tail_)
	{
		XMFLOAT3 tailRot = a->GetRotation();

		tailRot.z += cosf(PI * 2.0f / restrictionAngle * (waveTimer_));

		if (!tailDestructionFlag_)
		{
			a->SetRotation(tailRot);
		}
	}

	waveTimer_++;
}

void Monster::Assault()
{
	int count = 10;
	int life = 60;
	XMFLOAT3 bodyRot = body_[0]->GetRotation();
	float addAngle = 10.0f;

	bodyRot.x += addAngle;

	if (bodyRot.x >= 360.0f)
	{
		bodyRot.x = 0.0f;
	}

	body_[0]->SetRotation(bodyRot);

	XMFLOAT3 rot = rightForeFoot_[0]->GetRotation();

	rot.z -= addAngle;

	if (rot.z <= -90.0f)
	{
		rot.z = -90.0f;
	}

	rightForeFoot_[0]->SetRotation(rot);
	leftForeFoot_[0]->SetRotation(rot);
	leftHindFoot_[0]->SetRotation(rot);
	rightHindFoot_[0]->SetRotation(rot);
	bubbleEmitter_->SetCenter(4.0f);

	bubbleEmitter_->BubbleAdd(count, life, rightForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	bubbleEmitter_->BubbleAdd(count, life, leftForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	bubbleEmitter_->BubbleAdd(count, life, leftHindFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	bubbleEmitter_->BubbleAdd(count, life, rightHindFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
}

void Monster::TailAttack()
{
	int count = 10;
	int life = 60;
	float timeRate = 0.0f;
	int countNum = 30;
	timeRate = easeTimer_ / countNum;
	easeTimer_++;

	body_[0]->SetRotation(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, {}, { 0.0f,360.0f,0.0f }, timeRate));

	if (easeTimer_ > countNum)
	{
		isEaseFlag_ = true;
	}

	if (!tailDestructionFlag_)
	{
		bubbleEmitter_->SetCenter(2.5f);
		for (auto& a : tail_)
		{
			bubbleEmitter_->BubbleAdd(count, life, a->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		}
	}
}

void Monster::RightPunch()
{
	int count = 10;
	int life = 60;
	float timeRate = 0.0f;

	if (!isPunch_)
	{
		int countNum = 20;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		rightForeFoot_[0]->SetRotation(Ease::Action(EaseType::In, EaseFunctionType::Quad, {}, { 0.0f, 45.0f, 100.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isPunch_ = true;
		}
	}
	else if (isPunch_)
	{
		int countNum = 10;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		rightForeFoot_[0]->SetRotation(Ease::Action(EaseType::Out, EaseFunctionType::Quad, { 0.0f, 45.0f, 100.0f }, { 0.0f, -90.0f, 0.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isPunch_ = false;
			isEaseFlag_ = true;
		}
		bubbleEmitter_->SetCenter(1.0f);
		bubbleEmitter_->BubbleAdd(count, life, rightForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	}
}

void Monster::LeftPunch()
{
	int count = 10;
	int life = 60;
	float timeRate = 0.0f;

	if (!isPunch_)
	{
		int countNum = 20;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		leftForeFoot_[0]->SetRotation(Ease::Action(EaseType::In, EaseFunctionType::Quad, {}, { 0.0f, 45.0f, -100.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isPunch_ = true;
		}
	}
	else if (isPunch_)
	{
		int countNum = 10;
		timeRate = easeTimer_ / countNum;
		easeTimer_++;

		leftForeFoot_[0]->SetRotation(Ease::Action(EaseType::Out, EaseFunctionType::Quad, { 0.0f, 45.0f, 100.0f }, { 0.0f, -90.0f, 0.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isPunch_ = false;
			isEaseFlag_ = true;
		}
		bubbleEmitter_->SetCenter(1.0f);
		bubbleEmitter_->BubbleAdd(count, life, leftForeFoot_[2]->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
	}
}

void Monster::Waiting()
{
	float timeRate = 0.0f;
	int countNum = 30;
	timeRate = easeTimer_ / countNum;
	easeTimer_++;

	float range = 0.3f;

	if (!isEaseFlag_)
	{
		body_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, 0.0f + range, 0.0f }, { 0.0f, 0.0f - range, 0.0f }, timeRate));

		rightForeFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, 0.0f, -1.0f }, { 0.0f, range, -1.0f }, timeRate));
		leftForeFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, 0.0f,  1.0f }, { 0.0f, range,  1.0f }, timeRate));
		leftHindFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { -5.5f, 0.0f,  1.0f }, { -5.5f, range,  1.0f }, timeRate));
		rightHindFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { -5.5f, 0.0f, -1.0f }, { -5.5f, range, -1.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isEaseFlag_ = true;
		}
	}
	else if (isEaseFlag_)
	{
		body_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, 0.0f - range, 0.0f }, { 0.0f, 0.0f + range, 0.0f }, timeRate));

		rightForeFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, range, -1.0f }, { 0.0f, 0.0f, -1.0f }, timeRate));
		leftForeFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { 0.0f, range,  1.0f }, { 0.0f, 0.0f,  1.0f }, timeRate));
		leftHindFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { -5.5f, range,  1.0f }, { -5.5f, 0.0f,  1.0f }, timeRate));
		rightHindFoot_[0]->SetPosition(Ease::Action(EaseType::InOut, EaseFunctionType::Quad, { -5.5f, range, -1.0f }, { -5.5f, 0.0f, -1.0f }, timeRate));

		if (easeTimer_ > countNum)
		{
			easeTimer_ = 0.0f;
			isEaseFlag_ = false;
		}
	}
}

void Monster::Tornado()
{
	XMFLOAT3 rot = nucleus_->GetRotation();

	rot.x = 0.0f;
	rot.y += 10.0f;
	rot.z = 0.0f;

	if (rot.y >= 360)
	{
		rot.y = 0.0f;
	}

	nucleus_->SetRotation(rot);


	int count = 10;
	int life = 60;

	if (!tailDestructionFlag_)
	{
		bubbleEmitter_->SetCenter(2.5f);
		for (auto& a : tail_)
		{
			bubbleEmitter_->BubbleAdd(count, life, a->GetWorldPosition(), ObjFactory::GetInstance()->GetModel("bubble"));
		}
	}
}
