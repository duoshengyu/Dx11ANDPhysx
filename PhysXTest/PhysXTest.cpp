#include "PhysXTest.h"
#include "DirectInputClass.h"
#include <fstream>
std::ofstream debug("debug.txt");
extern DirectInput* gDInput;

bool g_pause = true;
//----------------------------------------------------------------------
//App initialize£¬include read and compile shader, define vertex layout
//create geometry objects, vertex buffer, index buffer,
//initialize camera, material and light
//----------------------------------------------------------------------
void PhysXTest::OnInit()
{
	HRESULT hr;
	ID3DBlob* pVSBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	//compile shader
	CompileShader(L"shader.hlsl", "VS", "vs_5_0", &pVSBlob);
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	// Compile the pixel shader
	CompileShader(L"shader.hlsl", "PS", "ps_5_0", &pPSBlob);
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return;
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return;

	// Set the input layout
	_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	/*
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(-1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, -1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, -1.0f, 1.0f) }
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Create index buffer
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return;

	// Set index buffer
	_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	*/
	//
	// Pack vertices of all the meshes into one vertex buffer.
	//
	MyMeshData::MeshData    ball;
	MyMeshData::MeshData	box;
	MyMeshData::MeshData    ground;
	MyMeshData::CreateSphere(0.5f, 10, 10, ball);
	MyMeshData::CreateBox(1.0f, 1.0f, 1.0f, box);
	MyMeshData::CreatePlane(50, 50, 10, ground);
	//record offset
	mBoxVertexOffset = 0;
	mBallVertexOffset = box.Vertices.size();
	mGroundVertexOffset = mBallVertexOffset + ball.Vertices.size();

	mBoxIndexOffset = 0;
	mBallIndexOffset = box.Indices.size();
	mGroundIndexOffset = mBallIndexOffset + ball.Indices.size();

	mBoxIndexCount = box.Indices.size();
	mBallIndexCount = ball.Indices.size();
	mGroundIndexCount = ground.Indices.size();


	UINT totalVertexCount = ball.Vertices.size() + box.Vertices.size() + ground.Vertices.size();
	UINT totalIndexCount = ball.Indices.size() + box.Indices.size() + ground.Indices.size();


	std::vector<MyMeshData::Vertex> vertices;

	for (UINT i = 0; i < box.Vertices.size(); i++)
	{
		vertices.push_back(box.Vertices[i]);
	}
	for (UINT i = 0; i < ball.Vertices.size(); i++)
	{
		vertices.push_back(ball.Vertices[i]);
	}
	for (UINT i = 0; i < ground.Vertices.size(); i++)
	{
		vertices.push_back(ground.Vertices[i]);
	}

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(MyMeshData::Vertex) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertices[0];
	hr = _pd3dDevice->CreateBuffer(&vbd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return;

	// Set vertex buffer
	UINT stride = sizeof(MyMeshData::Vertex);
	UINT offset = 0;
	_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<USHORT> indices;
	for (UINT i = 0; i < box.Indices.size(); i++)
	{
		indices.push_back(box.Indices[i]);
	}
	for (UINT i = 0; i < ball.Indices.size(); i++)
	{
		indices.push_back(ball.Indices[i]);
	}

	for (UINT i = 0; i < ground.Indices.size(); i++)
	{
		indices.push_back(ground.Indices[i]);
	}


	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &indices[0];
	hr = _pd3dDevice->CreateBuffer(&ibd, &initData, &g_pIndexBuffer);
	if (FAILED(hr))
		return;
	// Set index buffer
	_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Create the constant buffer for transform
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return;


	// Initialize the world matrix
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(20.0f, 20.0f, 20.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//g_View = XMMatrixLookAtLH(Eye, At, Up);
	_camera = new Camera(Eye, At, Up);
	// Initialize the projection matrix
	//g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, _aspectRatio, 0.01f, 100.0f);
	_camera->setLens(XM_PIDIV2, _aspectRatio, 0.01f, 100.0f);

	g_DirLight.Ambient = XMFLOAT4(0.1f, 0.22f, 0.42f, 1.0f);
	g_DirLight.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	g_DirLight.Direction = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Material.Ambient = XMFLOAT4(0.5f, 0.78f, 0.98f, 1.0f);
	g_Material.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	g_Material.Reflect = XMFLOAT4(0.3f, 0.5f, 0.7f, 1.0f);

}
//--------------------------------------------------------------------
// Update camera, and random shoot ball
//--------------------------------------------------------------------
void PhysXTest::OnUpdate()
{
	//get I/O input
	gDInput->poll();
	// Update our time
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	if (gDInput->keyDown(DIK_P))
	{
		g_pause = !g_pause;
	}
	
	_camera->update(t);
	//update camera
	if(g_pause)
	{ 	
		if(t >= 1.0f/60.0f)
		{
			g_myPxWorld.Simulate();
			dwTimeStart = 0;

			int num = rand() % 10 + 1;
			if(num < 5 && num > 2)
			g_myPxWorld.createDynamic(PxTransform(PxVec3(rand() % 8, 4 * (rand()%5), 20)), PxSphereGeometry(0.5), PxVec3(0, -30, -60));
		}
	}

	cb.mDirLight = g_DirLight;
	cb.mMaterial = g_Material;
	cb.mEyeW = XMFLOAT3(20.0f, 20.0f, 20.0f);
	//_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
}
//--------------------------------------------------------------------
// render all objects
//--------------------------------------------------------------------
void PhysXTest::OnRender()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f }; //red,green,blue,alpha
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

	_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC);

	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC, (PxActor**)&actors[0], nbActors);
		RenderPxActors(&actors[0], (PxU32)actors.size());
	}


	//
	// Renders a cube
	//
	//_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	//_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	//_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//_pImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);        // 36 vertices needed for 12 triangles in a triangle list
	//
	//XMMATRIX mWorld = XMMatrixTranslation(0.0f, 2.0f, 0.0f) * g_World;
	//
	//cb.mWorld = XMMatrixTranspose(mWorld);
	//cb.mWorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(0, XMMatrixTranspose(mWorld)));
	//cb.mWorldViewProjection = XMMatrixTranspose(g_Projection) * XMMatrixTranspose(g_View) * XMMatrixTranspose(mWorld);
	//_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	//_pImmediateContext->DrawIndexed(mBallIndexCount, mBallIndexOffset, mBallVertexOffset);    
	//
	//mWorld = XMMatrixTranslation(0.0f, -2.0f, 0.0f) * g_World;
	//
	//cb.mWorld = XMMatrixTranspose(mWorld);
	//cb.mWorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(0, XMMatrixTranspose(mWorld)));
	//cb.mWorldViewProjection = XMMatrixTranspose(g_Projection) * XMMatrixTranspose(g_View) * XMMatrixTranspose(mWorld);
	//_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	//_pImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);

	_pSwapChain->Present(0, 0);
}
//--------------------------------------------------------------------
// render objects in PhysX
//--------------------------------------------------------------------
void PhysXTest::RenderPxActors(PxRigidActor** actors, const PxU32 numActors)
{
	PxShape* shapes[128];
	for (PxU32 i = 0; i<numActors; i++)
	{
		const PxU32 nbShapes = actors[i]->getNbShapes();
		PX_ASSERT(nbShapes <= 128);
		actors[i]->getShapes(shapes, nbShapes);
		bool sleeping = actors[i]->isRigidDynamic() ? actors[i]->isRigidDynamic()->isSleeping() : false;

		for (PxU32 j = 0; j<nbShapes; j++)
		{
			//actors world space transform   *   the pose of the shape in actor space
			const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
			PxGeometryHolder h = shapes[j]->getGeometry();

			if (shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
				continue;

			// render object
			XMFLOAT4X4 mw;
			mw(0,0) = shapePose(0,0); mw(0,1) = shapePose(0,1); mw(0,2) = shapePose(0,2); mw(0,3) = shapePose(0,3);
			mw(1,0) = shapePose(1,0); mw(1,1) = shapePose(1,1); mw(1,2) = shapePose(1,2); mw(1,3) = shapePose(1,3);
			mw(2,0) = shapePose(2,0); mw(2,1) = shapePose(2,1); mw(2,2) = shapePose(2,2); mw(2,3) = shapePose(2,3);
			mw(3,0) = shapePose(3,0); mw(3,1) = shapePose(3,1); mw(3,2) = shapePose(3,2); mw(3,3) = shapePose(3,3);

			XMMATRIX mWorld = XMLoadFloat4x4(&mw);
			RenderGeometry(h , mWorld);

		}
	}
}
//--------------------------------------------------------------------
// render object
//--------------------------------------------------------------------
void PhysXTest::RenderGeometry(const PxGeometryHolder& h , XMMATRIX& mWorld)
{
	switch (h.getType())
	{
	case PxGeometryType::eBOX:
	{
		cb.mWorld = mWorld;
		cb.mWorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(0, mWorld));
		//cb.mWorldViewProjection = XMMatrixTranspose(g_Projection) * XMMatrixTranspose(g_View) * mWorld;
		cb.mWorldViewProjection = XMMatrixTranspose(_camera->proj()) * XMMatrixTranspose(_camera->view()) * mWorld;
		_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
		_pImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);

	}
	break;
	case PxGeometryType::eSPHERE:
	{
		cb.mWorld = mWorld;
		cb.mWorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(0, mWorld));
		//cb.mWorldViewProjection = XMMatrixTranspose(g_Projection) * XMMatrixTranspose(g_View) * mWorld;
		cb.mWorldViewProjection = XMMatrixTranspose(_camera->proj()) * XMMatrixTranspose(_camera->view()) * mWorld;
		_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
		_pImmediateContext->DrawIndexed(mBallIndexCount, mBallIndexOffset, mBallVertexOffset);
	}
	break;
	case PxGeometryType::ePLANE:
	{
		//cb.mWorld = mWorld;
		//cb.mWorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(0, mWorld));
		//cb.mWorldViewProjection = XMMatrixTranspose(g_Projection) * XMMatrixTranspose(g_View) * mWorld;
		//_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
		//_pImmediateContext->DrawIndexed(mGroundIndexCount, mGroundIndexOffset, mGroundVertexOffset); 
	}
	break;
	default:
		break;
	}
}
void PhysXTest::OnDestroy()
{
	SAFE_RELEASE(g_pConstantBuffer);
	SAFE_RELEASE(g_pIndexBuffer);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(g_pVertexBuffer);
}

bool PhysXTest::OnEvent(MSG msg)
{
	return true;
}
