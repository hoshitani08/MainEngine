#include "WinApp.h"

const wchar_t WinApp::WINDOW_CLASS_NAME[] = L"SEAHUNTER";

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}

void WinApp::CreateGameWindow()
{
	// �E�B���h�E�N���X�̐ݒ�
	wndClass_.cbSize = sizeof(WNDCLASSEX);
	wndClass_.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W��
	wndClass_.lpszClassName = WINDOW_CLASS_NAME; // �E�B���h�E�N���X��
	wndClass_.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	wndClass_.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	RegisterClassEx(&wndClass_); // �E�B���h�E�N���X��OS�ɓo�^

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow
	(
		wndClass_.lpszClassName, // �N���X��
		WINDOW_CLASS_NAME, // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW, // �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT, // �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT, // �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left, // �E�B���h�E����
		wrc.bottom - wrc.top, // �E�B���h�E�c��
		nullptr, // �e�E�B���h�E�n���h��
		nullptr, // ���j���[�n���h��
		wndClass_.hInstance, // �Ăяo���A�v���P�[�V�����n���h��
		nullptr // �I�v�V����
	);

	// �E�B���h�E�\��
	ShowWindow(hwnd_, SW_SHOW);
}

void WinApp::TerminateGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{}; // ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}