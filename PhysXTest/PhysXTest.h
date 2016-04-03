#ifndef PHYSXTEST_H_
#define PHYSXTEST_H_
//------------------------------------------------------------------------------------
// APP.h
//------------------------------------------------------------------------------------
#include "MYAPP.h"
#include "GeometryData.h"
#include "PhysXBasic.h"
#include "Camera.h"

using namespace DirectX;

class PhysXTest : public MYAPP
{
public:
	PhysXTest(std::wstring windowName, UINT windowWidth, UINT windowHeight, HINSTANCE hInstance, int nCmdShow)
		:MYAPP(windowName, windowWidth, windowHeight, hInstance, nCmdShow)
	{
		OnInit();
		g_myPxWorld.InitWorld();
		g_myPxWorld.InitObjects();
	};
	~PhysXTest()
	{
	}
	//light
	struct DirectionalLight
	{
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT3 Direction;
	};
	//material
	struct Material
	{
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Reflect;
	};
	//dx11 constant buffer
	struct ConstantBuffer
	{
		XMMATRIX mWorldViewProjection;
		XMMATRIX mWorld;
		XMMATRIX mWorldInverseTranspose;

		DirectionalLight mDirLight;
		Material         mMaterial;
		XMFLOAT3		 mEyeW;
	};
	//rewrite framework function
	void OnInit();
	void OnUpdate();
	void OnRender();
	void OnDestroy();
	bool OnEvent(MSG msg);

	void RenderPxActors(PxRigidActor** actors, const PxU32 numActors);
	void RenderGeometry(const PxGeometryHolder& h, XMMATRIX& mWorld);
private:
	//DX11 
	ID3D11VertexShader*     g_pVertexShader = NULL;
	ID3D11PixelShader*      g_pPixelShader = NULL;
	ID3D11InputLayout*      g_pVertexLayout = NULL;
	ID3D11Buffer*           g_pVertexBuffer = NULL;
	ID3D11Buffer*           g_pIndexBuffer = NULL;
	ID3D11Buffer*           g_pConstantBuffer = NULL;

	XMMATRIX                g_World;
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;
	Camera					*_camera;

	DirectionalLight        g_DirLight;
	Material				g_Material;

	ConstantBuffer			cb;
	//vertex and index buffer offset
	int						mBoxVertexOffset;
	int						mBallVertexOffset;
	int						mGroundVertexOffset;
	int                     mBoxIndexOffset;
	int						mBallIndexOffset;
	int						mGroundIndexOffset;
	int						mBoxIndexCount;
	int						mBallIndexCount;
	int						mGroundIndexCount;
	//physx world
	PhysXWorld              g_myPxWorld;
};

#endif