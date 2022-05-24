#pragma once
class Gamepad
{
public:
	enum Buttons {
		A, B, X, Y,
		DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT,
		LSHOULDER, RSHOULDER, LTHUMB, RTHUMB
	};
	Gamepad(int player);
	XINPUT_STATE getState();
	bool isConnected();
	void update();
	void vibration(float leftMotor, float rightMotor);
	bool isButtonPressed(int button);
private:
	int playerId;
	XINPUT_STATE state;
};

