#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//�������i��x�����s�������j
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput_, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

#pragma region �L�[�{�[�h
	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput_->CreateDevice(GUID_SysKeyboard, &devkeyboard_, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard_->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	//�r�����䃌�x���̃Z�b�g
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
#pragma region �}�E�X
	// �}�E�X�f�o�C�X�̐���	
	result = dinput_->CreateDevice(GUID_SysMouse, &devMouse_, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse_->SetDataFormat(&c_dfDIMouse2); // �W���`��
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devMouse_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}
#pragma endregion
#pragma region �Q�[���p�b�h
	//�������i��x�����s�������j
	result = DirectInput8Create
	(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinputPad_, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// �f�o�C�X�̗�
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
		// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
		result = devGamePad_->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// �����[�h���Βl���[�h�Ƃ��Đݒ�
		//ZeroMemory(&diprop, sizeof(diprop));
		diprop_.diph.dwSize = sizeof(diprop_);
		diprop_.diph.dwHeaderSize = sizeof(diprop_.diph);
		diprop_.diph.dwHow = DIPH_DEVICE;
		diprop_.diph.dwObj = 0;
		diprop_.dwData = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)
		// �����[�h��ύX
		result = devGamePad_->SetProperty(DIPROP_AXISMODE, &diprop_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// X���̒l�͈̔͐ݒ�
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

		// Y���̒l�͈̔͐ݒ�
		diprg_.diph.dwObj = DIJOFS_Y;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// Z���̒l�͈̔͐ݒ�
		diprg_.diph.dwObj = DIJOFS_Z;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// RX���̒l�͈̔͐ݒ�
		diprg_.diph.dwObj = DIJOFS_RX;
		result = devGamePad_->SetProperty(DIPROP_RANGE, &diprg_.diph);
		if (FAILED(result))
		{
			assert(0);
			return result;
		}

		// RY���̒l�͈̔͐ݒ�
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

#pragma region �L�[�{�[�h
	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard_->Acquire();
	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre_, key_, sizeof(key_));
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard_->GetDeviceState(sizeof(key_), key_);
#pragma endregion
#pragma region �}�E�X
	//�}�E�X���̎擾�J�n
	result = devMouse_->Acquire();
	// �O��̃L�[���͂�ۑ�
	memcpy(&mouseStatePre_, &mouseState_, sizeof(mouseState_));
	//�}�E�X�̓��͏�Ԃ��擾����
	result = devMouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
#pragma endregion
#pragma region �Q�[���p�b�h
	if (padFlag_ == true)
	{
		// ����J�n
		result = devGamePad_->Acquire();
		// �O��̃L�[���͂�ۑ�
		memcpy(&padDataPre_, &padData_, sizeof(padData_));
		// �Q�[���p�b�h�̓��͏��擾
		result = devGamePad_->GetDeviceState(sizeof(padData_), &padData_);
	}

	// �O��̃L�[���͂�ۑ�
	memcpy(&statePre_, &state_, sizeof(state_));
	XInputGetState(0, &state_);
	Vibration();
#pragma endregion
}

bool Input::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key_[keyNumber])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre_[keyNumber] && key_[keyNumber])
	{
		return true;
	}

	// �g���K�[�łȂ�
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
	// 0�łȂ���Ή����Ă���
	if (padData_.rgbButtons[keyNumber])
	{
		return true;
	}
	else
	{
		// �p�x�𗘗p�������@
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

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerPadKey(PadKey keyNumber)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!padDataPre_.rgbButtons[keyNumber] && padData_.rgbButtons[keyNumber])
	{
		return true;
	}
	else
	{
		// �p�x�𗘗p�������@
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

	// �g���K�[�łȂ�
	return false;
}

bool Input::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState_.rgbButtons[0])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState_.rgbButtons[2])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseRight()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState_.rgbButtons[1])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre_.rgbButtons[0] && mouseState_.rgbButtons[0])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre_.rgbButtons[2] && mouseState_.rgbButtons[2])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseRight()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre_.rgbButtons[1] && mouseState_.rgbButtons[1])
	{
		return true;
	}

	// �g���K�[�łȂ�
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
