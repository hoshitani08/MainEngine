#pragma once

#include <Windows.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	/// <returns></returns>
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
	/// <summary>
	/// 左スティックのY軸のトリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool PadStickTriggerY();
	/// <summary>
	/// 左スティックの傾き
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 PadStickGradient();
	/// <summary>
	/// 右スティックの傾き
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 PadRightStickGradient();
	/// <summary>
	/// 左スティックの角度
	/// </summary>
	/// <returns></returns>
	double PadStickAngle();
	/// <summary>
	/// 右スティックの角度
	/// </summary>
	/// <returns></returns>
	double PadRightStickAngle();
	/// <summary>
	/// 振動のフラグ設定
	/// </summary>
	/// <param name="_vibration">振動させるか</param>
	void SetVibration(bool _vibration) { vibrationFlag_ = _vibration; }
	/// <summary>
	/// 振動の強さの設定
	/// </summary>
	/// <param name="_vibrationPower">振動の強さ</param>
	void SetVibrationPower(int _vibrationPower) { vibrationPower_ = _vibrationPower; }
	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns></returns>
	bool TriggerPadLeft();
	/// <summary>
	/// キーの右ボタントリガーをチェック
	/// </summary>
	/// <returns></returns>
	bool TriggerPadRight();
	/// <summary>
	/// ゲームパッドのキーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( BUTTON_A 等)</param>
	/// <returns>押されているか</returns>
	bool PushPadKey(PadKey keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( BUTTON_A 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerPadKey(PadKey keyNumber);
	/// <summary>
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft();
	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle();
	/// <summary>
	/// キーの右ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseRight();
	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft();
	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle();
	/// <summary>
	/// キーの右ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseRight();
	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns></returns>
	MouseMove GetMouseMove();

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Input();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="ipddi"></param>
	/// <param name="pvRef"></param>
	/// <returns></returns>
	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
	/// <summary>
	/// 振動
	/// </summary>
	void Vibration();

public:
	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
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