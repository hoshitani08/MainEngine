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
	/// <summary>
	/// 仮ノードの行動の追加
	/// </summary>
	/// <param name="temp">関数</param>
	/// <param name="createType">まだ追加するか終了するかどうか</param>
	void AddNodeFunc(const std::function<bool()> temp, CreateType createType);
	/// <summary>
	/// 仮ノードのノードタイプの設定
	/// </summary>
	/// <param name="nodeType">ノードタイプ</param>
	void SetNodeFuncType(BehaviorTreeNode::NodeType nodeType) { saveFunc_->SetNodeType(nodeType); }
	/// <summary>
	/// 実行
	/// </summary>
	void Run();

private: // メンバ関数
	/// <summary>
	/// 仮ノードをノードに追加
	/// </summary>
	void AddNode();

private: // メンバ変数
	// 仮ノード
	std::unique_ptr<BehaviorTreeNode> saveFunc_;
	// ノード
	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};