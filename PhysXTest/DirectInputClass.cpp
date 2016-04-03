#include "SETTINGS.h"
#include "DirectInputClass.h"
#include "MYAPP.h"
extern MYAPP * g_app;
extern DirectInput* gDInput;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	//initiate a IDirectInput8 interface
	HR(DirectInput8Create(g_app->getAppInst(), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mDInput, 0));
	//Conduct keyboard device initialization
	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyboard->SetCooperativeLevel(g_app->getMainWnd(), keyboardCoopFlags));
	HR(mKeyboard->Acquire());
	//Conduct mouse device initialization
	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(g_app->getMainWnd(), mouseCoopFlags));
	HR(mMouse->Acquire());
}

DirectInput::~DirectInput()
{
	SAFE_RELEASE(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	SAFE_RELEASE(mKeyboard);
	SAFE_RELEASE(mMouse);
}

void DirectInput::poll()
{
	// Poll keyboard
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	if (FAILED(hr))
	{
		// if lost set to zero
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		// try to get keyboard
		hr = mKeyboard->Acquire();
	}

	// poll mouse
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&mMouseState, sizeof(mMouseState));
		hr = mMouse->Acquire();
	}
}
//is key has been press
bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}
//mouse button
bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

//-----------------------------------------------------------------------------
// mouse position
//-----------------------------------------------------------------------------

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}