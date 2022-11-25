#pragma once

#include <Windows.h>

class WinApp
{
public: // �ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const int WINDOW_WIDTH = 1280; // ����
	static const int WINDOW_HEIGHT = 720; // �c��
	static const wchar_t WINDOW_CLASS_NAME[];

public: // �ÓI�����o�֐�
	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="msg">���b�Z�[�W�ԍ�</param>
	/// <param name="wparam">���b�Z�[�W���1</param>
	/// <param name="lparam">���b�Z�[�W���2</param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �����o�֐�
	/// <summary>
	/// �Q�[���E�B���h�E�̍쐬
	/// </summary>
	void CreateGameWindow();
	/// <summary>
	/// �Q�[���E�B���h�E�̔j��
	/// </summary>
	void TerminateGameWindow();
	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();
	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd_; }
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	HINSTANCE GetInstance() { return wndClass_.hInstance; }

private: // �����o�ϐ�
	// Window�֘A
	HWND hwnd_ = nullptr; // �E�B���h�E�n���h��
	WNDCLASSEX wndClass_{}; // �E�B���h�E�N���X
};