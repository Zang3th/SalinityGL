#pragma once

#include <btBulletDynamicsCommon.h>
#include "Object.hpp"

class PhysicsEngine 
{
private:
	btBroadphaseInterface* _broadphase = nullptr;
	btDefaultCollisionConfiguration* _collisionConfiguration = nullptr;
	btCollisionDispatcher* _dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* _solver = nullptr;
	btDiscreteDynamicsWorld* _dynamicsWorld = nullptr;

	void init() 
	{
		//Init physics
		_broadphase = new btDbvtBroadphase();
		_collisionConfiguration = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
		_solver = new btSequentialImpulseConstraintSolver();
		_dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);

		//Configure settings
		_dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	}

public:
	PhysicsEngine() 
	{
		init();
	}

	~PhysicsEngine()
	{
		delete _broadphase;
		delete _collisionConfiguration;
		delete _dispatcher;
		delete _solver;
		delete _dynamicsWorld;
	}
	
	btRigidBody* addSphere(const glm::vec3& position, const float& size, const btScalar& mass, const float& restitution, const float& friction)
	{
		//Create shape
		btCollisionShape* sphereShape = new btSphereShape(size);
		btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
		btVector3 sphereInertia(0, 0, 0);
		sphereShape->calculateLocalInertia(mass, sphereInertia);

		//Create rigid body
		btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia);
		btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
		
		//Configure rigid body
		sphereRigidBody->setRestitution(restitution);
		sphereRigidBody->setFriction(friction);

		//Add rigid body to physics simulation
		_dynamicsWorld->addRigidBody(sphereRigidBody);

		return sphereRigidBody;
	}

	btRigidBody* addBox(const glm::vec3& position, const glm::vec3& size, const btScalar& mass, const float& restitution, const float& friction) 
	{
		//Create shape
		btCollisionShape* boxShape = new btBoxShape(btVector3(size.x, size.y, size.z));
		btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));		

		//Create rigid body
		btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, boxMotionState, boxShape, btVector3(0, 0, 0));
		btRigidBody* boxRigidBody = new btRigidBody(boxRigidBodyCI);

		//Configure rigid body
		boxRigidBody->setRestitution(restitution);
		boxRigidBody->setFriction(friction);
		boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

		//Add rigid body to physics simulation
		_dynamicsWorld->addRigidBody(boxRigidBody);

		return boxRigidBody;
	}

	void simulate(const float& dt) const
	{
		_dynamicsWorld->stepSimulation(dt);
	}
};