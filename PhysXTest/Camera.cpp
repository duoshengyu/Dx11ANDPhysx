#include "Camera.h"
#include "DirectInputClass.h"
#include <fstream>
extern std::ofstream debug;
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Camera::Camera()
{
	//Set identity matrix
	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();
	mView = XMMatrixIdentity();

	mEyes = XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f);
	mLookat = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mUp= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//change Sensitivity
	mSpeed = 50.0f;
}
Camera::Camera(XMVECTOR eye, XMVECTOR lookat, XMVECTOR up)
{
	//Set identity matrix
	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();
	mView = XMMatrixIdentity();

	lookAt(eye, lookat, up);
}


const XMMATRIX& Camera::world() const
{
	return mWorld;
}
const XMMATRIX& Camera::view() const
{
	return mView;
}

const XMMATRIX& Camera::proj() const
{
	return mProj;
}

const XMMATRIX& Camera::viewProj() const
{
	return mViewProj;
}

const XMVECTOR& Camera::up() const
{
	return mUp;
}

const XMVECTOR& Camera::look() const
{
	return mLookat;
}

XMVECTOR& Camera::pos()
{
	return mEyes;
}
//-----------------------------------------------------------------------------
// LookAt implement input(eye, target, up)
//-----------------------------------------------------------------------------
void Camera::lookAt(XMVECTOR& pos, XMVECTOR& target, XMVECTOR& up)
{
	//set member variable
	mEyes = pos;
	mLookat = target;
	mUp = up;
	mSpeed = 1;
	//build view
	mView = XMMatrixLookAtLH(pos, target, up);
}
//-----------------------------------------------------------------------------
// project matrix
//-----------------------------------------------------------------------------
void Camera::setLens(float fov, float aspect, float nearZ, float farZ)
{
	mProj = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	mViewProj = mView * mProj;
}

void Camera::setSpeed(float s)
{
	mSpeed = s;
}
//-----------------------------------------------------------------------------
// update camera position and direction
//-----------------------------------------------------------------------------
void Camera::update(float dt)
{
	//get look direction(L), right vector of camera space(R) and UP(U)
	XMVECTOR L = mLookat - mEyes;
	L = XMVector3Normalize(L);

	XMVECTOR R;
	R = XMVector3Cross(mUp, L);
	R = XMVector3Normalize(R);

	XMVECTOR U;
	U = XMVector3Cross(L, R);
	U = XMVector3Normalize(U);
	//press w s a d change diffrent vector
	XMVECTOR dir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	if (gDInput->keyDown(DIK_W))
	{
		dir += L;
		debug << L.m128_f32[0] << " " << L.m128_f32[1] << " " << L.m128_f32[2] << " " << L.m128_f32[3] << std::endl;
	}

	if (gDInput->keyDown(DIK_S))
		dir -= L;
	if (gDInput->keyDown(DIK_D))
		dir += R;
	if (gDInput->keyDown(DIK_A))
		dir -= R;

	//change position of camera
	dir = XMVector3Normalize(dir);
	mEyes = mEyes + dir*mSpeed*dt;
	mLookat = mLookat + dir*mSpeed*dt;

	//change orientation when push the right button of mouse
	float pitch = 0.0f;
	float yAngle = 0.0f;
	if (gDInput->mouseButtonDown(0))
	{
		pitch = gDInput->mouseDY() / 150.0f;
		yAngle = gDInput->mouseDX() / 150.0f;
	
	// rotate camera with right vector
	XMMATRIX vRotate;
	vRotate = XMMatrixRotationAxis(R, pitch);
	XMVECTOR oldL = mEyes - mLookat;
	XMVECTOR newL = XMVector3TransformCoord(oldL, vRotate);
	//XMVECTOR newU = XMVector3TransformCoord(U, vRotate);

	//rotate with y axis(turn left or right)
	vRotate = XMMatrixRotationAxis(U, yAngle);
	
	newL = XMVector3TransformCoord(newL, vRotate);
	//newU = XMVector3TransformCoord(newU, vRotate);
	//
	mEyes = mLookat + newL;
	}
	// rebuild view matrix
	mView = XMMatrixLookAtLH(mEyes, mLookat, mUp);

}