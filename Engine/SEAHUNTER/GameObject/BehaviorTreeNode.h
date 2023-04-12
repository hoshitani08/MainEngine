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
	enum class NodeType
	{
		Selector,
		Sequence,
	};

public: // メンバ関数
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	/// <summary>
	/// 行動の追加
	/// </summary>
	/// <param name="temp">関数</param>
	void AddFunc(const std::function<bool()> temp);
	/// <summary>
	/// 実行
	/// </summary>
	/// <returns>成功or失敗</returns>
	bool Run();
	/// <summary>
	/// ノードのタイプの設定
	/// </summary>
	/// <param name="nodeType">ノードタイプ</param>
	void SetNodeType(NodeType nodeType) { nodeType_ = nodeType; }

private: // メンバ変数
	// ノードタイプ
	NodeType nodeType_ = NodeType::Selector;
	// 行動
	std::vector<std::function<bool()>> func_;
};