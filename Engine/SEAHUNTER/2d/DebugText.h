#pragma once

#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#include "Sprite.h"

class DebugText final
{
private: // �G�C���A�X
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int MAX_CHAR_COUNT = 256; // �ő啶����
	static const int FONT_WIDTH = 9; // �t�H���g�摜��1�������̉���
	static const int FONT_HEIGHT = 18; // �t�H���g�摜��1�������̏c��
	static const int FONT_LINECOUNT = 14; // �t�H���g�摜��1�s���̕�����

public:// �ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static DebugText* GetInstance();

public:// �����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texnumber">�e�N�X�`���n���h��</param>
	void Initialize(UINT texnumber);
	/// <summary>
	/// ������ǉ�
	/// </summary>
	/// <param name="text">������</param>
	/// <param name="x">�\�����WX</param>
	/// <param name="y">�\�����WY</param>
	/// <param name="size">�{��</param>
	void Print(const std::string& text, float x, float y, float size);
	/// <summary>
	/// ������+�ϐ��ǉ�
	/// </summary>
	/// <param name="x">�\�����WX</param>
	/// <param name="y">�\�����WY</param>
	/// <param name="text">������</param>
	/// <param name="i">�ϐ�</param>
	/// <param name="size">�{��</param>
	void VariablePrint(float x, float y, const std::string& text, float i, float size);
	/// <summary>
	/// �`��t���b�V��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	void DrawAll(ID3D12GraphicsCommandList* cmdList);

	DebugText(const DebugText&) = delete;
	DebugText& operator=(const DebugText&) = delete;

private:
	DebugText();
	~DebugText();

private:
	// �X�v���C�g�f�[�^�̔z��
	std::unique_ptr<Sprite> spriteDatas_[MAX_CHAR_COUNT];
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex_ = 0;
	// �ϐ���`�悷�邽�߂̕ۑ�
	XMVECTOR variable_ = {};
};