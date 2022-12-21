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
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	/// <returns></returns>
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);
	/// <summary>
	/// ���X�e�B�b�N��Y���̃g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool PadStickTriggerY();
	/// <summary>
	/// ���X�e�B�b�N�̌X��
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 PadStickGradient();
	/// <summary>
	/// �E�X�e�B�b�N�̌X��
	/// </summary>
	/// <returns></returns>
	XMFLOAT2 PadRightStickGradient();
	/// <summary>
	/// ���X�e�B�b�N�̊p�x
	/// </summary>
	/// <returns></returns>
	double PadStickAngle();
	/// <summary>
	/// �E�X�e�B�b�N�̊p�x
	/// </summary>
	/// <returns></returns>
	double PadRightStickAngle();
	/// <summary>
	/// �U���̃t���O�ݒ�
	/// </summary>
	/// <param name="_vibration">�U�������邩</param>
	void SetVibration(bool _vibration) { vibrationFlag_ = _vibration; }
	/// <summary>
	/// �U���̋����̐ݒ�
	/// </summary>
	/// <param name="_vibrationPower">�U���̋���</param>
	void SetVibrationPower(int _vibrationPower) { vibrationPower_ = _vibrationPower; }
	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns></returns>
	bool TriggerPadLeft();
	/// <summary>
	/// �L�[�̉E�{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns></returns>
	bool TriggerPadRight();
	/// <summary>
	/// �Q�[���p�b�h�̃L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( BUTTON_A ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushPadKey(PadKey keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( BUTTON_A ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerPadKey(PadKey keyNumber);
	/// <summary>
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft();
	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle();
	/// <summary>
	/// �L�[�̉E�{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseRight();
	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft();
	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle();
	/// <summary>
	/// �L�[�̉E�{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseRight();
	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns></returns>
	MouseMove GetMouseMove();

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Input();
	/// <summary>
	/// �f�X�g���N�^
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
	/// �U��
	/// </summary>
	void Vibration();

public:
	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
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