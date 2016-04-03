#ifndef SETTINGS_H_
#define SETTINGS_H_
//--------------------------------------------------------------------------------------
// This file includes common **.h files and some help functions
//--------------------------------------------------------------------------------------

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <wrl.h>
#include <string>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <iostream>
//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
// input(filename, entry function name, version, (out)result)
//--------------------------------------------------------------------------------------
static void CompileShader(WCHAR* fileName, LPCSTR entryName, LPCSTR ShaderVersion, ID3DBlob** result)
{
	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	HRESULT hr = D3DX11CompileFromFile(fileName, NULL, NULL, entryName, ShaderVersion,
		dwShaderFlags, 0, NULL, result, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return;
	}
	if (pErrorBlob) pErrorBlob->Release();
}

#ifndef HR						//easy return error message
#define HR(x)												   \
	{														   \
	HRESULT hr = x;											   \
if (FAILED(hr))												   \
		{													   \
		std::cerr <<__FILE__ <<" "<< __LINE__<< std::endl;	   \
		}													   \
	}
#endif   

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //easy to release point resource
#endif							

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //easy to release COM resource
#endif


#endif