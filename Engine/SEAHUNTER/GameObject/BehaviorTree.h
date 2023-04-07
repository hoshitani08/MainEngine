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
public: // サブクラス

	enum class CreateType
	{
		Add,
		End,
	};
public: // メンバ関数
	BehaviorTree();
	~BehaviorTree() = default;

	void AddNodeFunc(const std::function<bool()> temp, CreateType createType);
	void Run();

private: // メンバ関数
	void AddNode();

private: // メンバ変数

	std::unique_ptr<BehaviorTreeNode> saveFunc_;

	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};