#include "BehaviorTree.h"

BehaviorTree::BehaviorTree()
{
	saveFunc_ = std::make_unique<BehaviorTreeNode>();
}

void BehaviorTree::AddNodeFunc(const std::function<bool()> temp, CreateType createType)
{
	const std::function<bool()> tempFnuc = temp;

	saveFunc_->AddFunc(tempFnuc);

	if (createType == CreateType::End)
	{
		AddNode();
	}
}

void BehaviorTree::AddNode()
{
	func_.push_back(std::move(saveFunc_));
	saveFunc_ = std::make_unique<BehaviorTreeNode>();
}

void BehaviorTree::Run()
{
	for (auto& main : func_)
	{
		if (!main->Run())
		{
			break;
		}
	}
}
