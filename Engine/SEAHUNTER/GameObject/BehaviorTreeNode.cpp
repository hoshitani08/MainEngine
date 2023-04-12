#include "BehaviorTreeNode.h"

void BehaviorTreeNode::AddFunc(const std::function<bool()> temp)
{
	func_.push_back(temp);
}

bool BehaviorTreeNode::Run()
{
	for (auto& a : func_)
	{
		if (nodeType_ == NodeType::Selector)
		{
			if (a())
			{
				return false;
			}
		}
		else
		{
			if (!a())
			{
				return true;
			}
		}
	}

	if (nodeType_ == NodeType::Selector)
	{
		return true;
	}
	else
	{
		return false;
	}
}
