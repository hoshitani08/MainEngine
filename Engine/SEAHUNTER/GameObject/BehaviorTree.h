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
public: // �T�u�N���X

	enum class CreateType
	{
		Add,
		End,
	};
public: // �����o�֐�
	BehaviorTree();
	~BehaviorTree() = default;

	void AddNodeFunc(const std::function<bool()> temp, CreateType createType);
	void Run();

private: // �����o�֐�
	void AddNode();

private: // �����o�ϐ�

	std::unique_ptr<BehaviorTreeNode> saveFunc_;

	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};