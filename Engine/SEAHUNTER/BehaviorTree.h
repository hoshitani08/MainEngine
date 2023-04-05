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
public: // メンバ関数
	BehaviorTree() = default;
	~BehaviorTree() = default;

	void AddNode(BehaviorTreeNode* tempFunc);
	void Run();

private: // メンバ変数

	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};