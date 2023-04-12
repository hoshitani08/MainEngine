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
	enum class NodeType
	{
		Selector,
		Sequence,
	};

public: // �����o�֐�
	BehaviorTreeNode() = default;
	~BehaviorTreeNode() = default;

	/// <summary>
	/// �s���̒ǉ�
	/// </summary>
	/// <param name="temp">�֐�</param>
	void AddFunc(const std::function<bool()> temp);
	/// <summary>
	/// ���s
	/// </summary>
	/// <returns>����or���s</returns>
	bool Run();
	/// <summary>
	/// �m�[�h�̃^�C�v�̐ݒ�
	/// </summary>
	/// <param name="nodeType">�m�[�h�^�C�v</param>
	void SetNodeType(NodeType nodeType) { nodeType_ = nodeType; }

private: // �����o�ϐ�
	// �m�[�h�^�C�v
	NodeType nodeType_ = NodeType::Selector;
	// �s��
	std::vector<std::function<bool()>> func_;
};