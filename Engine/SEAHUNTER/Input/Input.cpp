#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//初期化（一度だけ行う処理）
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput_, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

#pragma region キーボード
	//キーボードデバイスの生成
	result = dinput_->CreateDevice(GUID_SysKeyboard, &devkeyboard_, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//入力データ形式のセット
	result = devkeyboard_->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//排他制御レベルのセット
	result = devkeyboard_->SetCooperativeLevel
	(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}
#pragma endregion
#pragma region マウス
	// マウスデバイスの生成	
	result = dinput_->CreateDevice(GUID_SysMouse, &devMouse_, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devMouse_->SetDataFormat(&c_dfDIMouse2); // 標準形式
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devMouse_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}
#pragma endregion
#pragma region ゲームパッド
	//初期化（一度だけ行う処理）
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinputPad_, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// デバイスの列挙
	if (FAILED(dinputPad_->EnumDevices(DI8DEVTYPE_GAMEPAD, DeviceFindCallBack, &parameter_, DIEDFL_ATTACHEDONLY)))
	{
		assert(0);
		return result;
	}

	result = dinputPad_->CreateDevice(GUID_Joystick, &devGamePad_, NULL);
	if (FAILED(result))
	{
		padFlag_ = false;
	}

	if (padFlag_ == true)
	{
		// デバイスのフォーマットの設定
		result = devGamePad_->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// 軸モードを絶対値モードとして設定
		//ZeroMemory(&diprop, sizeof(diprop));
		diprop_.diph.dwSize = sizeof(diprop_);
		diprop_.diph.dwHeaderSize = sizeof(diprop_.diph);
		diprop_.diph.dwHow = DIPH_DEVICE;
		diprop_.diph.dwObj = 0;
		diprop_.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)
		// 軸モードを変更
		result = devGamePad_->SetProperty(DIPROP_AXISMODE, &diprop_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// X軸の値の範囲設定
		ZeroMemory(&diprg_, sizeof(diprg_));
		diprg_.diph.dwSize = sizeof(diprg_);
		diprg_.diph.dwHeaderSize = sizeof(diprg_.diph);
		diprg_.diph.dwHow = DIPH_BYOFFSET;
		diprg_.diph.dwObj = DIJOFS_X;
		diprg_.lMin = -1000;
		diprg_.lMax = 1000;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// Y軸の値の範囲設定
		diprg_.diph.dwObj = DIJOFS_Y;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// Z軸の値の範囲設定
		diprg_.diph.dwObj = DIJOFS_Z;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// RX軸の値の範囲設定
		diprg_.diph.dwObj = DIJOFS_RX;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// RY軸の値の範囲設定
		diprg_.diph.dwObj = DIJOFS_RY;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		result = devGamePad_->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}
	}
#pragma endregion

	return true;
}

void Input::Update()
{
	HRESULT result;

#pragma region キーボード
	//キーボード情報の取得開始
	result = devkeyboard_->Acquire();
	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));
	//全キーの入力状態を取得する
	result = devkeyboard_->GetDeviceState(sizeof(key_), key_);
#pragma endregion
#pragma region マウス
	//マウス情報の取得開始
	result = devMouse_->Acquire();
	// 前回のキー入力を保存
	memcpy(&mouseStatePre_, &mouseState_, sizeof(mouseState_));
	//マウスの入力状態を取得する
	result = devMouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
#pragma endregion
#pragma region ゲームパッド
	if (padFlag_ == true)
	{
		// 制御開始
		result = devGamePad_->Acquire();
		// 前回のキー入力を保存
		memcpy(&padDataPre_, &padData_, sizeof(padData_));
		// ゲームパッドの入力情報取得
		result = devGamePad_->GetDeviceState(sizeof(padData_), &padData_);
	}

	// 前回のキー入力を保存
	memcpy(&statePre_, &state_, sizeof(state_));
	XInputGetState(0, &state_);
	Vibration();
#pragma endregion
}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (key_[keyNumber])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre_[keyNumber] && key_[keyNumber])
	{
		return true;
	}

	// トリガーでない
	return false;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

XMFLOAT2& Input::PadStickGradient()
{
	float x = padData_.lX / 1000.0f;
	float y = padData_.lY / 1000.0f;

	if (fabsf(x) < 0.2f && fabsf(x) > -0.2f)
	{
		x = 0.0f;
	}
	if (fabsf(y) < 0.2f && fabsf(y) > -0.2f)
	{
		y = 0.0f;
	}

	return XMFLOAT2(x, y);
}

XMFLOAT2& Input::PadRightStickGradient()
{
	float x = padData_.lRx / 1000.0f;
	float y = padData_.lRy / 1000.0f;

	if (fabsf(x) < 0.2f && fabsf(x) > -0.2f)
	{
		x = 0.0f;
	}
	if (fabsf(y) < 0.2f && fabsf(y) > -0.2f)
	{
		y = 0.0f;
	}

	return XMFLOAT2(x, y);
}

double Input::PadStickAngle()
{
	double radian = atan2(padData_.lY - 0, padData_.lX - 0);
	return radian * (180.0f / 3.14159265359f);
}

double Input::PadRightStickAngle()
{
	double radian = atan2(padData_.lRy - 0, padData_.lRx - 0);
	return radian * (180.0f / 3.14159265359f);
}

bool Input::TriggerPadLeft()
{
	if (padData_.lZ > angle_)
	{
		return true;
	}

	return false;
}

bool Input::TriggerPadRight()
{
	if (padData_.lZ < -angle_)
	{
		return true;
	}

	return false;
}

bool Input::PushPadKey(PadKey keyNumber)
{
	// 0でなければ押している
	if (padData_.rgbButtons[keyNumber])
	{
		return true;
	}
	else
	{
		// 角度を利用した方法
		float mainRad = XMConvertToRadians((padData_.rgdwPOV[0] / 100.0f));
		float sabRad = XMConvertToRadians((padDataPre_.rgdwPOV[0] / 100.0f));

		float x = sinf(mainRad);
		float y = cosf(mainRad);

		if (keyNumber == BUTTON_DPAD_UP && y > 0.01f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_DOWN && y < -0.7f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_LEFT && x < -0.8f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_RIGHT && x > 0.01f)
		{
			return true;
		}
	}

	// 押していない
	return false;
}

bool Input::TriggerPadKey(PadKey keyNumber)
{
	// 前回が0で、今回が0でなければトリガー
	if (!padDataPre_.rgbButtons[keyNumber] && padData_.rgbButtons[keyNumber])
	{
		return true;
	}
	else
	{
		// 角度を利用した方法
		float mainRad = XMConvertToRadians((padData_.rgdwPOV[0] / 100.0f));
		float sabRad = XMConvertToRadians((padDataPre_.rgdwPOV[0] / 100.0f));

		float x = sinf(mainRad);
		float x2 = sinf(sabRad);

		float y = cosf(mainRad);
		float y2 = cosf(sabRad);

		if (keyNumber == BUTTON_DPAD_UP && y > 0.01f && y2 < 0.01f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_DOWN && y < -0.7f && y2 > -0.7f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_LEFT && x < -0.8f && x2 > -0.8f)
		{
			return true;
		}
		else if (keyNumber == BUTTON_DPAD_RIGHT && x > 0.01f && x2 < 0.01f)
		{
			return true;
		}
	}

	// トリガーでない
	return false;
}

bool Input::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState_.rgbButtons[0])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState_.rgbButtons[2])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::PushMouseRight()
{
	// 0でなければ押している
	if (mouseState_.rgbButtons[1])
	{
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre_.rgbButtons[0] && mouseState_.rgbButtons[0])
	{
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre_.rgbButtons[2] && mouseState_.rgbButtons[2])
	{
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseRight()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre_.rgbButtons[1] && mouseState_.rgbButtons[1])
	{
		return true;
	}

	// トリガーでない
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp = {};
	tmp.lX = mouseState_.lX;
	tmp.lY = mouseState_.lY;
	tmp.lZ = mouseState_.lZ;
	return tmp;
}

Input::Input()
{
}

Input::~Input()
{
}

void Input::Vibration()
{
	if (!vibrationFlag_)
	{
		vibration_.wLeftMotorSpeed = 0;
		XInputSetState(0, &vibration_);
	}
	else if (vibrationFlag_)
	{
		vibration_.wLeftMotorSpeed = vibrationPower_;
		XInputSetState(0, &vibration_);
	}
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}
