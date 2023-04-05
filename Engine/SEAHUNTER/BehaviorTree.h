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
public: // �����o�֐�
	BehaviorTree() = default;
	~BehaviorTree() = default;

	void AddNode(BehaviorTreeNode* tempFunc);
	void Run();

private: // �����o�ϐ�

	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};