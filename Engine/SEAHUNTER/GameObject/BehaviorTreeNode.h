#pragma once

#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

class BehaviorTreeNode
{
public: // メンバ関数
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	void AddFunc(const std::function<bool()> temp);
	bool Run();

private: // メンバ変数

	std::vector<std::function<bool()>> func_;

	//std::vector<std::unique_ptr<BehaviorTreeNode>> nodeFunc_;
};