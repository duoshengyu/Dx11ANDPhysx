//------------------------------------------------------------------------------
//Deal with I/O of directx 
//------------------------------------------------------------------------------
#ifndef DIRECT_INPUT_H
#define DIRECT_INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class DirectInput
{
public:
	DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();							//get I/O state
	bool keyDown(char key);
	bool mouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:

	DirectInput(const DirectInput& rhs);

private:
	//device
	IDirectInput8*       mDInput;
	//keyboard
	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256];
	//mouse
	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;
};
extern DirectInput* gDInput;

#endif 