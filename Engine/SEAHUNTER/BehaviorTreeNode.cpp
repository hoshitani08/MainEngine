#include "BehaviorTreeNode.h"

void BehaviorTreeNode::AddFunc(const std::function<bool()> temp, NodeType type)
{
	nodeType_ = type;
	func_ = temp;

	/*AddFunc([this]() {
		Run();
		});*/
}

void BehaviorTreeNode::AddNode(BehaviorTreeNode* tempFunc)
{
	nodeFunc_.push_back(std::move(std::unique_ptr<BehaviorTreeNode>(tempFunc)));
}

bool BehaviorTreeNode::Run()
{

	if (nodeType_ == BehaviorTreeNode::NodeType::Selector)
	{
		if (func_())
		{
			return false;
		}
	}
	if (nodeType_ == BehaviorTreeNode::NodeType::Sequence)
	{
		if (!func_())
		{
			return false;
		}
	}

	for (auto& node : nodeFunc_)
	{
		node->Run();
	}

	return true;
}