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
public: // サブクラス
	enum class ActivityType
	{
		STANDBY, // 待機
		RUNNING, // 実行中
		SUCCESS, // 成功
		FAILURE, // 失敗
	};

	enum class NodeType
	{
		Selector, // セレクター
		Sequence, // シーケンス
	};

public: // メンバ関数
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	void AddFunc(const std::function<bool()> temp, NodeType type);
	void AddNode(BehaviorTreeNode* tempFunc);
	bool Run();

	NodeType GetNodeType() { return nodeType_; }

private: // メンバ変数

	NodeType nodeType_;

	ActivityType actType_ = ActivityType::STANDBY;

	std::function<bool()> func_;

	std::vector<std::unique_ptr<BehaviorTreeNode>> nodeFunc_;
};

