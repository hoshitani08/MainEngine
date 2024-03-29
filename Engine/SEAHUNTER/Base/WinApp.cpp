#include "WinApp.h"

const wchar_t WinApp::WINDOW_CLASS_NAME[] = L"SEAHUNTER";

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void WinApp::CreateGameWindow()
{
	// ウィンドウクラスの設定
	wndClass_.cbSize = sizeof(WNDCLASSEX);
	wndClass_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャ
	wndClass_.lpszClassName = WINDOW_CLASS_NAME; // ウィンドウクラス名
	wndClass_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	wndClass_.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	RegisterClassEx(&wndClass_); // ウィンドウクラスをOSに登録

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow
	(
		wndClass_.lpszClassName, // クラス名
		WINDOW_CLASS_NAME, // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT, // 表示X座標（OSに任せる）
		CW_USEDEFAULT, // 表示Y座標（OSに任せる）
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		wndClass_.hInstance, // 呼び出しアプリケーションハンドル
		nullptr // オプション
	);

	// ウィンドウ表示
	ShowWindow(hwnd_, SW_SHOW);
}

void WinApp::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}