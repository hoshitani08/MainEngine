#include "BehaviorTreeNode.h"

void BehaviorTreeNode::AddFunc(const std::function<bool()> temp)
{
	func_.push_back(temp);
}

bool BehaviorTreeNode::Run()
{
	for (auto& a : func_)
	{
		if (a())
		{
			return false;
		}
	}

	return true;
}
