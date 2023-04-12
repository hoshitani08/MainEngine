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
	/// <summary>
	/// ���m�[�h�̍s���̒ǉ�
	/// </summary>
	/// <param name="temp">�֐�</param>
	/// <param name="createType">�܂��ǉ����邩�I�����邩�ǂ���</param>
	void AddNodeFunc(const std::function<bool()> temp, CreateType createType);
	/// <summary>
	/// ���m�[�h�̃m�[�h�^�C�v�̐ݒ�
	/// </summary>
	/// <param name="nodeType">�m�[�h�^�C�v</param>
	void SetNodeFuncType(BehaviorTreeNode::NodeType nodeType) { saveFunc_->SetNodeType(nodeType); }
	/// <summary>
	/// ���s
	/// </summary>
	void Run();

private: // �����o�֐�
	/// <summary>
	/// ���m�[�h���m�[�h�ɒǉ�
	/// </summary>
	void AddNode();

private: // �����o�ϐ�
	// ���m�[�h
	std::unique_ptr<BehaviorTreeNode> saveFunc_;
	// �m�[�h
	std::vector<std::unique_ptr<BehaviorTreeNode>> func_;
};