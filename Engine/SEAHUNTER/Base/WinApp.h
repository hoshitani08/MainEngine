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
	// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �����o�֐�
	// �Q�[���E�B���h�E�̍쐬
	void CreateGameWindow();
	// �Q�[���E�B���h�E�̔j��
	void TerminateGameWindow();
	// ���b�Z�[�W�̏���
	bool ProcessMessage();
	// �E�B���h�E�n���h���̎擾
	HWND GetHwnd() { return hwnd_; }

	HINSTANCE GetInstance() { return wndClass_.hInstance; }

private: // �����o�ϐ�
	// Window�֘A
	HWND hwnd_ = nullptr; // �E�B���h�E�n���h��
	WNDCLASSEX wndClass_{}; // �E�B���h�E�N���X
};