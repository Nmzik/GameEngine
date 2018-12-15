#include "CVehicle.h"

#define CUBE_HALF_EXTENTS 1
btVector3 wheelDirectionCS0(0, 0, -1);
btVector3 wheelAxleCS(1, 0, 0);

float gVehicleSteering = 0.f;
float steeringIncrement = 0.04f;
float steeringClamp = 0.3f;
float wheelRadius = 0.5f;
float wheelWidth = 0.4f;
float wheelFriction = 1000; // BT_LARGE_FLOAT;
float suspensionStiffness = 50.f;
float suspensionDamping = 2.3f;
float suspensionCompression = 4.4f;
float rollInfluence = 0.1f; // 1.0f;

btScalar suspensionRestLength(0.6f);
btScalar m_defaultContactProcessingThreshold(BT_LARGE_FLOAT);

CVehicle::CVehicle(glm::vec3 position, float mass, YftLoader* yft)
	: throttle(0)
	, steeringValue(0)
	, vehicle(yft)
	, Entity(position, glm::quat(-1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f))
{
	btRaycastVehicle::btVehicleTuning m_tuning;

	m_tuning.m_frictionSlip = 3.f;

	chassisShape = new btBoxShape(btVector3(1.f, 2.f, 0.5f));
	compound = new btCompoundShape();
	btTransform localTrans;
	localTrans.setIdentity();
	//	localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0.f, 0.f, 0.3f));

	compound->addChildShape(localTrans, chassisShape);

	btVector3 localInertia(0, 0, 0);
	compound->calculateLocalInertia(mass, localInertia);

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(position.x, position.y, position.z));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(tr);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, compound, localInertia);

	m_carChassis = new btRigidBody(cInfo);
	m_carChassis->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

	m_carChassis->setWorldTransform(tr);
	PhysicsSystem::dynamicsWorld->addRigidBody(m_carChassis, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::KinematicFilter);

	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	m_vehicleRayCaster = myNew btDefaultVehicleRaycaster(PhysicsSystem::dynamicsWorld);
	m_vehicle = myNew btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);

	///	never deactivate the vehicle
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);

	//	choose coordinate system
	m_vehicle->setCoordinateSystem(0, 2, 1);

	PhysicsSystem::dynamicsWorld->addAction(m_vehicle);

	float connectionHeight = 0.5f;

	bool isFrontWheel = true;

	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3f * wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f * wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	isFrontWheel = false;

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f * wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3f * wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}
}

CVehicle::~CVehicle()
{
	PhysicsSystem::dynamicsWorld->removeAction(m_vehicle);
	PhysicsSystem::dynamicsWorld->removeRigidBody(m_carChassis);

	delete m_carChassis->getMotionState();
	delete m_carChassis;
	delete chassisShape;
	delete compound;
	delete m_wheelShape;
	delete m_vehicleRayCaster;
	m_vehicleRayCaster = NULL;
	delete m_vehicle;
	m_vehicle = NULL;
}

glm::mat4 CVehicle::GetMat4()
{
	m_carChassis->getWorldTransform().getOpenGLMatrix(&modelMatrix[0][0]);

	return modelMatrix;
}

void CVehicle::PhysicsTick()
{
	float engineForce = throttle * 5000.0f;

	m_vehicle->applyEngineForce(engineForce, 0);
	m_vehicle->applyEngineForce(engineForce, 1);

	m_vehicle->setSteeringValue(steeringValue, 0);
	m_vehicle->setSteeringValue(steeringValue, 1);
}