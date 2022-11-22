#pragma once

#include <Windows.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <dinput.h>
#include <xinput.h>

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "xinput.lib")

enum  PadKey
{
	BUTTON_A, // Aボタン
	BUTTON_B, // Bボタン
	BUTTON_X, // Xボタン
	BUTTON_Y, // Yボタン
	BUTTON_LEFT_SHOULDER, // LBボタン
	BUTTON_RIGHT_SHOULDER, // RBボタン
	BUTTON_BACK, // BACKボタン
	BUTTON_START, // STARTボタン
	BUTTON_LEFT_THUMB, // 左スティック押し込み
	BUTTON_RIGHT_THUMB, // 右スティック押し込み
	BUTTON_DPAD_UP, // デジタル方向ボタン上
	BUTTON_DPAD_DOWN, // デジタル方向ボタン下
	BUTTON_DPAD_LEFT, // デジタル方向ボタン左
	BUTTON_DPAD_RIGHT // デジタル方向ボタン右
};

class Input final
{
private:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	struct MouseMove
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

public: //メンバ関数
	//初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

	//スティックの傾き
	XMFLOAT2& PadStickGradient();
	XMFLOAT2& PadRightStickGradient();
	//スティックの角度
	double PadStickAngle();
	double PadRightStickAngle();
	//振動のフラグ設定
	void SetVibration(bool _vibration) { vibrationFlag_ = _vibration; }
	void SetVibrationPower(int _vibrationPower) { vibrationPower_ = _vibrationPower; }
	// キーの左ボタントリガーをチェック
	bool TriggerPadLeft();
	// キーの右ボタントリガーをチェック
	bool TriggerPadRight();
	//ゲームパッドのキーの押下をチェック
	bool PushPadKey(PadKey keyNumber);
	// キーのトリガーをチェック
	bool TriggerPadKey(PadKey keyNumber);

	// キーの左ボタン押下をチェック
	bool PushMouseLeft();
	// キーの中ボタン押下をチェック
	bool PushMouseMiddle();
	// キーの右ボタン押下をチェック
	bool PushMouseRight();
	// キーの左ボタントリガーをチェック
	bool TriggerMouseLeft();
	// キーの中ボタントリガーをチェック
	bool TriggerMouseMiddle();
	// キーの右ボタントリガーをチェック
	bool TriggerMouseRight();
	// マウス移動量を取得
	MouseMove GetMouseMove();

private:
	Input();
	~Input();

	void Vibration();

public:
	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;

	static Input* GetInstance();

private: //メンバ変数
	//キーボード
	ComPtr<IDirectInput8> dinput_;
	ComPtr<IDirectInputDevice8> devkeyboard_;
	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};

	//dゲームパッド
	ComPtr<IDirectInput8> dinputPad_;
	ComPtr<IDirectInputDevice8> devGamePad_;
	LPVOID parameter_;
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop_;
	// 値の範囲設定
	DIPROPRANGE diprg_;
	// 無反応範囲
	float angle_ = 200;
	// 入力情報
	DIJOYSTATE padData_;
	DIJOYSTATE padDataPre_;
	//接続確認
	bool padFlag_ = true;
	//xゲームパッド
	XINPUT_STATE state_;
	XINPUT_STATE statePre_;
	bool vibrationFlag_ = false;
	XINPUT_VIBRATION vibration_;
	int vibrationPower_ = 65535;

	//マウス
	ComPtr<IDirectInputDevice8> devMouse_;
	DIMOUSESTATE2 mouseState_ = {};
	DIMOUSESTATE2 mouseStatePre_ = {};
};