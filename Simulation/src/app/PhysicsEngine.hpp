#pragma once

#include <btBulletDynamicsCommon.h>
#include "Object.hpp"
#include <map>

unsigned int PHYSIC_BODY_INDEX = 0;

class PhysicsEngine 
{
private:
	btBroadphaseInterface* _broadphase = nullptr;
	btDefaultCollisionConfiguration* _collisionConfiguration = nullptr;
	btCollisionDispatcher* _dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* _solver = nullptr;
	btDiscreteDynamicsWorld* _dynamicsWorld = nullptr;
	btCollisionShape* _boxShape = nullptr, * _sphereShape = nullptr;
	std::map<unsigned int, btRigidBody*> _physicBodies;

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

		//Preconfigure sphere shape -> can be reused
		_sphereShape = new btSphereShape(1.0f);
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
		delete _boxShape;
		delete _sphereShape;

		for (auto const& body : _physicBodies)
			delete body.second;
	}
	
	unsigned int addSphere(const glm::vec3& position, const btScalar& mass, const float& restitution, const float& friction)
	{
		//Create shape
		btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
		btVector3 sphereInertia(0, 0, 0);
		_sphereShape->calculateLocalInertia(mass, sphereInertia);

		//Create rigid body
		btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, _sphereShape, sphereInertia);
		btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
		
		//Configure rigid body
		sphereRigidBody->setRestitution(restitution);
		sphereRigidBody->setFriction(friction);

		//Add rigid body to physics simulation
		_dynamicsWorld->addRigidBody(sphereRigidBody);

		//Add rigid body to container to keep track of it
		unsigned int currentIndex = PHYSIC_BODY_INDEX;
		_physicBodies[PHYSIC_BODY_INDEX] = sphereRigidBody;
		PHYSIC_BODY_INDEX++;

		return currentIndex;
	}

	unsigned int addBox(const glm::vec3& position, const glm::vec3& size, const btScalar& mass, const float& restitution, const float& friction) 
	{
		//Create shape
		_boxShape = new btBoxShape(btVector3(size.x, size.y, size.z));
		btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));		

		//Create rigid body
		btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, boxMotionState, _boxShape, btVector3(0, 0, 0));
		btRigidBody* boxRigidBody = new btRigidBody(boxRigidBodyCI);

		//Configure rigid body
		boxRigidBody->setRestitution(restitution);
		boxRigidBody->setFriction(friction);
		boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

		//Add rigid body to physics simulation
		_dynamicsWorld->addRigidBody(boxRigidBody);

		//Add rigid body to container to keep track of it
		unsigned int currentIndex = PHYSIC_BODY_INDEX;
		_physicBodies[PHYSIC_BODY_INDEX] = boxRigidBody;
		PHYSIC_BODY_INDEX++;

		return currentIndex;
	}

	glm::mat4 getWorldTransform(const unsigned int& physicIndex) 
	{
		btTransform t;
		_physicBodies[physicIndex]->getMotionState()->getWorldTransform(t);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), t.getRotation().getAngle(), glm::vec3(t.getRotation().getAxis().getX(), t.getRotation().getAxis().getY(), t.getRotation().getAxis().getZ()));
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ()));
		
		return translation * rotation;
	}

	void removeFromSimulation(const unsigned int& physicIndex)
	{
		_dynamicsWorld->removeRigidBody(_physicBodies[physicIndex]);
	}

	void simulate(const float& dt) const
	{
		_dynamicsWorld->stepSimulation(dt);
	}
};