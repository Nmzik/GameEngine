#include "Vehicle.h"

#define CUBE_HALF_EXTENTS 1
btVector3 wheelDirectionCS0(0, 0, -1);
btVector3 wheelAxleCS(1, 0, 0);

float	gVehicleSteering = 0.f;
float	steeringIncrement = 0.04f;
float	steeringClamp = 0.3f;
float	wheelRadius = 0.5f;
float	wheelWidth = 0.4f;
float	wheelFriction = 1000;//BT_LARGE_FLOAT;
float	suspensionStiffness = 50.f;
float	suspensionDamping = 2.3f;
float	suspensionCompression = 4.4f;
float	rollInfluence = 0.1f;//1.0f;

btScalar suspensionRestLength(0.6f);
btScalar m_defaultContactProcessingThreshold(BT_LARGE_FLOAT);

Vehicle::Vehicle(glm::vec3 position, YftLoader* yft, btDiscreteDynamicsWorld* world) : throttle(0), steeringValue(0), vehicle(yft)
{
	btRaycastVehicle::btVehicleTuning m_tuning;
	chassisShape = new btBoxShape(btVector3(1.f, 2.f, 0.5f));
	compound = new btCompoundShape();
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0, 0, 0.3));

	compound->addChildShape(localTrans, chassisShape);

	btVector3 localInertia(0, 0, 0);
	compound->calculateLocalInertia(800, localInertia);

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(position.x, position.y, position.z));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(tr);

	btRigidBody::btRigidBodyConstructionInfo cInfo(800, myMotionState, compound, localInertia);

	m_carChassis = new btRigidBody(cInfo);
	m_carChassis->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

	m_carChassis->setWorldTransform(tr);
	world->addRigidBody(m_carChassis);

	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	m_vehicleRayCaster = new btDefaultVehicleRaycaster(world);
	m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);

	///never deactivate the vehicle
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);

	//choose coordinate system
	m_vehicle->setCoordinateSystem(0, 2, 1);

	world->addVehicle(m_vehicle);

	float connectionHeight = 0.5f;

	bool isFrontWheel = true;

	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3*wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);


	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);


	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);
	isFrontWheel = false;
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);
	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3*wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);

	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i<m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}


}

Vehicle::~Vehicle()
{
	delete m_carChassis->getMotionState();
	delete m_carChassis;
	delete chassisShape;
	delete compound;
	delete m_wheelShape;
	delete m_vehicleRayCaster;
	m_vehicleRayCaster = NULL;
	delete m_vehicle;
	m_vehicle = NULL;

	delete vehicle;
}

glm::mat4 Vehicle::GetMat4()
{
	glm::mat4 model;

	m_carChassis->getWorldTransform().getOpenGLMatrix(&model[0][0]);

	return model;
}

void Vehicle::PhysicsTick()
{
	float engineForce = throttle * 5000.0f;

	m_vehicle->applyEngineForce(engineForce, 0);
	m_vehicle->applyEngineForce(engineForce, 1);

	m_vehicle->setSteeringValue(steeringValue, 0);
	m_vehicle->setSteeringValue(steeringValue, 1);
}

void Vehicle::Draw() 
{
	vehicle->Draw();
}