#include "PhysXBasic.h"

MyErrorCallback gDefaultErrorCallback;
MyAllocator gDefaultAllocatorCallback;

//-----------------------------------------------------------------------------
// PhysX initiate
//-----------------------------------------------------------------------------
void PhysXWorld::InitWorld()
{
	_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,gDefaultErrorCallback);

	if (!_Foundation)
		fatalError("PxCreateFoundation failed!");

	//The ProfileZoneManager is needed if you want to 
	//use the performance profiling facilities in PVD.  
	//Enabling profiling certainly has an impact on performance and memory and thus should be used
	//only in non-production builds.

	bool recordMemoryAllocations = true;
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_Foundation);

	if (!_profileZoneManager)
		fatalError("PxProfileZoneManager::createProfileZoneManager failed!");

	_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_Foundation,
		PxTolerancesScale(), recordMemoryAllocations, _profileZoneManager);
	if (!_Physics)
		fatalError("PxCreatePhysics failed!");

	// setup default material...
	dMaterial = _Physics->createMaterial(0.5f, 0.5f, 0.5f);
	if (!dMaterial)
		fatalError("createMaterial failed!");

	PxSceneDesc sceneDesc(_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = _Physics->createScene(sceneDesc);

	if (!gScene)
		fatalError("createScene failed!");
}
//-----------------------------------------------------------------------------
// create PhysX objects.(gound, and box) 
//-----------------------------------------------------------------------------
void PhysXWorld::InitObjects()
{
	//create ground
	PxRigidStatic* groundPlane = PxCreatePlane(*_Physics, PxPlane(0, 1, 0, 0), *dMaterial);
	gScene->addActor(*groundPlane);

	//create cubes
	PxShape* shape = _Physics->createShape(PxBoxGeometry(0.5, 0.5, 0.5), *dMaterial);
	for (PxU32 i = 0; i < 10; i++)
	{
		for (PxU32 j = 0; j < 10; j++)
		{
			for (PxU32 k = 0; k < 10; k++)
			{
				PxVec3 tr = PxVec3(PxReal(i), PxReal(j+0.5), PxReal(k));
			     PxTransform localTm(tr);
			     PxRigidDynamic* body = _Physics->createRigidDynamic(localTm);
			     body->attachShape(*shape);
			     PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			     gScene->addActor(*body);
			}

		}
	}
	shape->release();
}
//-----------------------------------------------------------------------------
// Add a object
//-----------------------------------------------------------------------------
void PhysXWorld::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*_Physics, t, geometry, *dMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
}
void PhysXWorld::Simulate()
{
	PX_UNUSED(true);
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}
void PhysXWorld::DestroyWorld()
{
	PX_UNUSED(true);
	gScene->release();
	_Physics->release();
	_profileZoneManager->release();
	_Foundation->release();
}
