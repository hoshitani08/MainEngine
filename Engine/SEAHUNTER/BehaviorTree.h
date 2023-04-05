#pragma once

#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

#include "BehaviorTreeNode.h"

class BehaviorTree
{
public: // ƒƒ“ƒoŠÖ”
	BehaviorTree() = default;
	~BehaviorTree() = default;

	void AddNode(BehaviorTreeNode* tempFunc);
	void Run();

private: // ƒƒ“ƒo•Ï”

	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};