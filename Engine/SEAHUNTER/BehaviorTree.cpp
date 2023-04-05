#include "BehaviorTree.h"

void BehaviorTree::AddNode(BehaviorTreeNode* tempFunc)
{
	func_.push_back(std::move(std::unique_ptr<BehaviorTreeNode>(tempFunc)));
}

void BehaviorTree::Run()
{

	for (auto& main : func_)
	{
		if (main->GetNodeType() == BehaviorTreeNode::NodeType::Selector)
		{
			if (main->Run())
			{
				break;
			}
		}
		if (main->GetNodeType() == BehaviorTreeNode::NodeType::Sequence)
		{
			if (!main->Run())
			{
				break;
			}
		}
		
	}
}
