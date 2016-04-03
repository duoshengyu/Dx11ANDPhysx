#include "PhysXTest.h"
#include "DirectInputClass.h"
MYAPP *g_app = NULL;
DirectInput* gDInput = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	g_app = new PhysXTest(L"DX11ANDPhysXTest", 800, 600, hInstance, nCmdShow);
	gDInput = new DirectInput(DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	return g_app->Run();
}