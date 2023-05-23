#pragma once
#pragma once
#include <XInput.h>
#include <Windows.h>
#pragma comment(lib, "XInput.lib")
namespace Controller
{
	class XboxController
	{
	private:
		XINPUT_STATE _controllerState;
		XINPUT_VIBRATION _controllervibration;
		XINPUT_GAMEPAD _controllergamepad;
		int _controllerNum;
	public:
		XboxController(int playerNumber);
		XINPUT_STATE GetControllerState();
		DWORD SetControllerVirbation(float intensity);
		DWORD SetControllerTriggers();
		bool CheckConnection();
	};

	XboxController::XboxController(int playerNumber)
	{
		_controllerNum = playerNumber - 1;
	}

	DWORD XboxController::SetControllerTriggers()
	{
		ZeroMemory(&_controllergamepad, sizeof(XINPUT_GAMEPAD));
		_controllergamepad.wButtons = _controllergamepad.wButtons;
		_controllergamepad.bLeftTrigger = 255; // Left trigger fully pressed
		_controllergamepad.bRightTrigger = 255; // Right trigger fully pressed
		XInputSetState(0, &_controllervibration);
		Sleep(5000);
		_controllergamepad.bLeftTrigger = 0; // Left trigger released
		_controllergamepad.bRightTrigger = 0; // Right trigger released
		return XInputSetState(0, &_controllervibration);
	}

	XINPUT_STATE XboxController::GetControllerState()
	{
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
		XInputGetState(_controllerNum, &_controllerState);
		return _controllerState;
	}

	DWORD XboxController::SetControllerVirbation(float intensity)
	{
		ZeroMemory(&_controllervibration, sizeof(XINPUT_VIBRATION));
		_controllervibration.wLeftMotorSpeed = intensity;
		_controllervibration.wRightMotorSpeed = intensity;
		XInputSetState(_controllerNum, &_controllervibration);
		_controllervibration.wLeftMotorSpeed = 0;
		_controllervibration.wRightMotorSpeed = 0;
		return XInputSetState(_controllerNum, &_controllervibration);
	}

	bool XboxController::CheckConnection()
	{
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState(_controllerNum, &_controllerState);

		if (Result == ERROR_SUCCESS)
			return true;

		return false;
	}

	XboxController* PlayerController = new XboxController(1);

	BOOL ControllerConnection() {
		if (PlayerController->CheckConnection())
			return TRUE;
		return FALSE;
	}

	BOOL IsPressingRightTrigger() {
		if (PlayerController->GetControllerState().Gamepad.bRightTrigger)
			return TRUE;
		return FALSE;
	}

	BOOL IsPressingLeftTrigger() {
		if (PlayerController->GetControllerState().Gamepad.bLeftTrigger)
			return TRUE;
		return FALSE;
	}

	BOOL SetControllerVibration(float intensity) {
		if (PlayerController->SetControllerVirbation(intensity) != ERROR_SUCCESS)
			return TRUE;
		return FALSE;
	}

	BOOL SetControllerTriggers() {
		if (PlayerController->SetControllerTriggers() != ERROR_SUCCESS)
			return TRUE;
		return FALSE;
	}
}