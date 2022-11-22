#pragma once

#include <Windows.h>

class WinApp
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int WINDOW_WIDTH = 1280; // 横幅
	static const int WINDOW_HEIGHT = 720; // 縦幅
	static const wchar_t WINDOW_CLASS_NAME[];

public: // 静的メンバ関数
	// ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	// ゲームウィンドウの作成
	void CreateGameWindow();
	// ゲームウィンドウの破棄
	void TerminateGameWindow();
	// メッセージの処理
	bool ProcessMessage();
	// ウィンドウハンドルの取得
	HWND GetHwnd() { return hwnd_; }

	HINSTANCE GetInstance() { return wndClass_.hInstance; }

private: // メンバ変数
	// Window関連
	HWND hwnd_ = nullptr; // ウィンドウハンドル
	WNDCLASSEX wndClass_{}; // ウィンドウクラス
};