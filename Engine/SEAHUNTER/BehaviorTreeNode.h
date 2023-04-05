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
public: // �T�u�N���X
	enum class ActivityType
	{
		STANDBY, // �ҋ@
		RUNNING, // ���s��
		SUCCESS, // ����
		FAILURE, // ���s
	};

	enum class NodeType
	{
		Selector, // �Z���N�^�[
		Sequence, // �V�[�P���X
	};

public: // �����o�֐�
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	void AddFunc(const std::function<bool()> temp, NodeType type);
	void AddNode(BehaviorTreeNode* tempFunc);
	bool Run();

	NodeType GetNodeType() { return nodeType_; }

private: // �����o�ϐ�

	NodeType nodeType_;

	ActivityType actType_ = ActivityType::STANDBY;

	std::function<bool()> func_;

	std::vector<std::unique_ptr<BehaviorTreeNode>> nodeFunc_;
};

