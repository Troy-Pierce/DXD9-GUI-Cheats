#include "pch.h"

static const WORD XINPUT_Buttons[] = {
XINPUT_GAMEPAD_A,
XINPUT_GAMEPAD_B,
XINPUT_GAMEPAD_X,
XINPUT_GAMEPAD_Y,
XINPUT_GAMEPAD_DPAD_UP,
XINPUT_GAMEPAD_DPAD_DOWN,
XINPUT_GAMEPAD_DPAD_LEFT,
XINPUT_GAMEPAD_DPAD_RIGHT,
XINPUT_GAMEPAD_LEFT_SHOULDER,
XINPUT_GAMEPAD_RIGHT_SHOULDER,
XINPUT_GAMEPAD_LEFT_THUMB,
XINPUT_GAMEPAD_RIGHT_THUMB
};

bool Gamepad::isButtonPressed(int button) {
	return this->isConnected() ? (this->state.Gamepad.wButtons & XINPUT_Buttons[button]) : false;
}

void Gamepad::vibration(float leftMotor, float rightMotor) {
	XINPUT_VIBRATION vibrationState;
	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));
	int iLeftMotor = int(leftMotor * 65535.0f);
	int iRightMotor = int(rightMotor * 65535.0f);
	vibrationState.wLeftMotorSpeed = iLeftMotor;
	vibrationState.wRightMotorSpeed = iRightMotor;
	XInputSetState(this->playerId, &vibrationState);
}

void Gamepad::update() {
	this->state = this->getState();
}

bool Gamepad::isConnected() {
	ZeroMemory(&this->state, sizeof(XINPUT_STATE));
	DWORD ret = XInputGetState(this->playerId, &this->state);
	return ret == ERROR_SUCCESS;
}

XINPUT_STATE Gamepad::getState() {
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(this->playerId, &state);
	return state;
}

Gamepad::Gamepad(int player) : playerId{ player } {}
