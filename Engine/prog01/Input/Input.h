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
	BUTTON_A, // A�{�^��
	BUTTON_B, // B�{�^��
	BUTTON_X, // X�{�^��
	BUTTON_Y, // Y�{�^��
	BUTTON_LEFT_SHOULDER, // LB�{�^��
	BUTTON_RIGHT_SHOULDER, // RB�{�^��
	BUTTON_BACK, // BACK�{�^��
	BUTTON_START, // START�{�^��
	BUTTON_LEFT_THUMB, // ���X�e�B�b�N��������
	BUTTON_RIGHT_THUMB, // �E�X�e�B�b�N��������
	BUTTON_DPAD_UP, // �f�W�^�������{�^����
	BUTTON_DPAD_DOWN, // �f�W�^�������{�^����
	BUTTON_DPAD_LEFT, // �f�W�^�������{�^����
	BUTTON_DPAD_RIGHT // �f�W�^�������{�^���E
};

class Input final
{
private:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	struct MouseMove
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

public: //�����o�֐�
	//������
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	//�X�V
	void Update();
	// �L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	// �L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

	//�X�e�B�b�N�̌X��
	XMFLOAT2& PadStickGradient();
	XMFLOAT2& PadRightStickGradient();
	//�X�e�B�b�N�̊p�x
	double PadStickAngle();
	double PadRightStickAngle();
	//�U���̃t���O�ݒ�
	void SetVibration(bool _vibration) { vibrationFlag_ = _vibration; }
	void SetVibrationPower(int _vibrationPower) { vibrationPower_ = _vibrationPower; }
	// �L�[�̍��{�^���g���K�[���`�F�b�N
	bool TriggerPadLeft();
	// �L�[�̉E�{�^���g���K�[���`�F�b�N
	bool TriggerPadRight();
	//�Q�[���p�b�h�̃L�[�̉������`�F�b�N
	bool PushPadKey(PadKey keyNumber);
	// �L�[�̃g���K�[���`�F�b�N
	bool TriggerPadKey(PadKey keyNumber);

	// �L�[�̍��{�^���������`�F�b�N
	bool PushMouseLeft();
	// �L�[�̒��{�^���������`�F�b�N
	bool PushMouseMiddle();
	// �L�[�̉E�{�^���������`�F�b�N
	bool PushMouseRight();
	// �L�[�̍��{�^���g���K�[���`�F�b�N
	bool TriggerMouseLeft();
	// �L�[�̒��{�^���g���K�[���`�F�b�N
	bool TriggerMouseMiddle();
	// �L�[�̉E�{�^���g���K�[���`�F�b�N
	bool TriggerMouseRight();
	// �}�E�X�ړ��ʂ��擾
	MouseMove GetMouseMove();

private:
	Input();
	~Input();

	void Vibration();

public:
	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;

	static Input* GetInstance();

private: //�����o�ϐ�
	//�L�[�{�[�h
	ComPtr<IDirectInput8> dinput_;
	ComPtr<IDirectInputDevice8> devkeyboard_;
	BYTE key_[256] = {};
	BYTE keyPre_[256] = {};

	//d�Q�[���p�b�h
	ComPtr<IDirectInput8> dinputPad_;
	ComPtr<IDirectInputDevice8> devGamePad_;
	LPVOID parameter_;
	// �����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop_;
	// �l�͈̔͐ݒ�
	DIPROPRANGE diprg_;
	// �������͈�
	float angle_ = 200;
	// ���͏��
	DIJOYSTATE padData_;
	DIJOYSTATE padDataPre_;
	//�ڑ��m�F
	bool padFlag_ = true;
	//x�Q�[���p�b�h
	XINPUT_STATE state_;
	XINPUT_STATE statePre_;
	bool vibrationFlag_ = false;
	XINPUT_VIBRATION vibration_;
	int vibrationPower_ = 65535;

	//�}�E�X
	ComPtr<IDirectInputDevice8> devMouse_;
	DIMOUSESTATE2 mouseState_ = {};
	DIMOUSESTATE2 mouseStatePre_ = {};
};