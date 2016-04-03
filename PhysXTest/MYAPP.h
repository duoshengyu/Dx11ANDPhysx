//------------------------------------------------------------------------------------
// Application template class.
// only thing you have to do is implement the virtual functions.
//------------------------------------------------------------------------------------
#ifndef MYAPP_H_
#define MYAPP_H_

#include "SETTINGS.h"

using namespace Microsoft::WRL;

class MYAPP
{
public:
	MYAPP(std::wstring windowName, UINT windowWidth, UINT windowHeight, HINSTANCE hInstance, int nCmdShow);
	~MYAPP();
	
	//init functions
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	static LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);
	
	int Run();

	//user defined functions

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
	virtual bool OnEvent(MSG msg) = 0;
	//access functions
	HINSTANCE getAppInst() { return _appInst; }
	HWND	  getMainWnd() { return _hwnd; }
protected:
	//d3d component
	HINSTANCE						  _appInst;
	HWND							  _hwnd;
	D3D_DRIVER_TYPE					  _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				  _featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*					  _pd3dDevice;
	ID3D11DeviceContext*			  _pImmediateContext;
	IDXGISwapChain*					  _pSwapChain;
	ID3D11RenderTargetView*			  _pRenderTargetView;

	//window
	UINT wndWidth;
	UINT wndHeight;
	std::wstring wndName;
	float _aspectRatio;
};

#endif