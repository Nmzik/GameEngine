#include "CVehicle.h"
//#include "../3rdparty/glm/glm/gtx/quaternion.hpp"

#define CUBE_HALF_EXTENTS 1

CVehicle::CVehicle(glm::vec3 position, float mass, YftLoader* yft, btDynamicsWorld* physicsWorld)
    : throttle(0)
    , steeringValue(0)
    , vehicle(yft)
    , CEntity(position, glm::quat(-1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f), ObjectType::Vehicle)
{
    btRaycastVehicle::btVehicleTuning m_tuning;

    m_tuning.m_frictionSlip = 3.f;

    chassisShape = std::make_unique<btBoxShape>(btVector3(1.f, 2.f, 0.5f));
    compound = std::make_unique<btCompoundShape>();
    btTransform localTrans;
    localTrans.setIdentity();
    //	localTrans effectively shifts the center of mass with respect to the chassis
    localTrans.setOrigin(btVector3(0.f, 0.f, 0.3f));

    compound->addChildShape(localTrans, chassisShape.get());

    btVector3 localInertia(0, 0, 0);
    compound->calculateLocalInertia(mass, localInertia);

    btTransform tr;
    tr.setIdentity();
    tr.setOrigin(btVector3(position.x, position.y, position.z));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(tr);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, compound.get(), localInertia);

    m_carChassis = std::make_unique<btRigidBody>(cInfo);
    m_carChassis->setContactProcessingThreshold(BT_LARGE_FLOAT);

    //m_wheelShape = std::make_unique<btCylinderShapeX>(btVector3(wheelWidth, wheelRadius, wheelRadius));

    m_vehicleRayCaster = std::make_unique<btDefaultVehicleRaycaster>(physicsWorld);
    m_vehicle = std::make_unique<btRaycastVehicle>(m_tuning, m_carChassis.get(), m_vehicleRayCaster.get());

    ///	never deactivate the vehicle
    m_carChassis->setActivationState(DISABLE_DEACTIVATION);

    //	choose coordinate system
    m_vehicle->setCoordinateSystem(0, 2, 1);

    float connectionHeight = 0.5f;

    bool isFrontWheel = true;
    float wheelRadius = 0.5f;
    float wheelWidth = 0.4f;
    btScalar suspensionRestLength(0.6f);
    btVector3 wheelDirectionCS0(0, 0, -1);
    btVector3 wheelAxleCS(1, 0, 0);

    btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3f * wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

    connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f * wheelWidth), 2 * CUBE_HALF_EXTENTS - wheelRadius, connectionHeight);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

    isFrontWheel = false;

    connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f * wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

    connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3f * wheelWidth), -2 * CUBE_HALF_EXTENTS + wheelRadius, connectionHeight);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, m_tuning, isFrontWheel);

    float gVehicleSteering = 0.f;
    float steeringIncrement = 0.04f;
    float steeringClamp = 0.3f;
    float wheelFriction = 1000;  // BT_LARGE_FLOAT;
    float suspensionStiffness = 50.f;
    float suspensionDamping = 2.3f;
    float suspensionCompression = 4.4f;
    float rollInfluence = 0.1f;  // 1.0f;

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
    delete m_carChassis->getMotionState();
}

void CVehicle::physicsTick()
{
    position = glm::vec3(m_carChassis->getWorldTransform().getOrigin().getX(),
                         m_carChassis->getWorldTransform().getOrigin().getY(),
                         m_carChassis->getWorldTransform().getOrigin().getZ());
    /*btTransform trans = m_carChassis->getWorldTransform();
    glm::vec3 position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    btQuaternion rot = trans.getRotation();
    glm::quat q = glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
    glm::mat4 rot4 = glm::toMat4(q);
    glm::mat4 m = glm::translate(glm::mat4(1.0), position) * rot4;*/

    //
    float engineForce = throttle * 5000.0f;

    m_vehicle->applyEngineForce(engineForce, 0);
    m_vehicle->applyEngineForce(engineForce, 1);

    m_vehicle->setSteeringValue(steeringValue, 0);
    m_vehicle->setSteeringValue(steeringValue, 1);
}