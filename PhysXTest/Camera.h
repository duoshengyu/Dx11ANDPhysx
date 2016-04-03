//------------------------------------------------------------------------------
//simple camera class
//note:: this camera have ¡°Gimbal Lock¡±£¨¥¸¥ó¥Ð¥ë¥í¥Ã¥¯£© problem
//------------------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#include "SETTINGS.h"
using namespace DirectX;
class Camera
{
public:
	Camera();
	Camera(XMVECTOR eye, XMVECTOR lookat, XMVECTOR up);
	~Camera() {}
	const XMMATRIX& view() const;					//get view matrix
	const XMMATRIX& proj() const;					//get project matrix
	const XMMATRIX& viewProj() const;				//get proj * view
	const XMMATRIX& world() const;				//get world matrix
	const XMVECTOR& up() const;
	const XMVECTOR& look() const;

	XMVECTOR& pos();

	void lookAt(XMVECTOR& pos, XMVECTOR& target, XMVECTOR& up);
	void setLens(float fov, float aspect, float nearZ, float farZ);
	void setSpeed(float s);

	void update(float dt);

protected:
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;
	XMMATRIX mViewProj;

	//camera vectors
	XMVECTOR mEyes;
	XMVECTOR mLookat;
	XMVECTOR mUp;


	float mSpeed;


};

#endif 