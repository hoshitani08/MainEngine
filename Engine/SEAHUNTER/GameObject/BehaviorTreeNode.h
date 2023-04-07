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
public: // ƒƒ“ƒoŠÖ”
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	void AddFunc(const std::function<bool()> temp);
	bool Run();

private: // ƒƒ“ƒo•Ï”

	std::vector<std::function<bool()>> func_;

	//std::vector<std::unique_ptr<BehaviorTreeNode>> nodeFunc_;
};