#pragma once

#include <array>
#include <memory>
#include <vector>
#include "CEntity.h"
#include <btBulletDynamicsCommon.h>

class CVehicle;
class YddLoader;
class YdrLoader;

struct CharacterWeaponSlot
{
    // Assuming these match the entries in weapon.dat
    uint32_t weaponId;
    uint32_t bulletsClip;
    uint32_t bulletsTotal;
};

class CPed : public CEntity
{
    std::unique_ptr<btCapsuleShapeZ> physShape;
    std::unique_ptr<btRigidBody> body;

    std::array<CharacterWeaponSlot, 13> weapons;
    uint16_t currentWeapon = 0;
    bool inWater = false;
    glm::vec3 playerDirection;
    YddLoader* player;
    CVehicle* vehicle;
    bool on_ground = false;
    glm::vec3 movementDirection;

public:
    CPed(glm::vec3 position, YddLoader* ydd);
    ~CPed();

    //CPed& operator=(CPed&& other) = default;
    //CPed(CPed&& other) = default;
    std::vector<YdrLoader*> playerModel;
    bool isAlive() const
    {
        return health > 0;
    }

    float getCurrentHealth() const
    {
        return health;
    }

    void setMovementDirection(glm::vec3 pos)
    {
        movementDirection = pos;
    }

    void setPosition(glm::vec3 pos);

    void takeDamage(float dmg);

    void physicsTick();

    void enterVehicle(CVehicle* nearestVehicle);
    void exitVehicle();

    /*void setGravity(glm::vec3 gravity)
    {
        body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }*/

    CVehicle* getCurrentVehicle() const
    {
        return vehicle;
    }
    void addToInventory(uint32_t slot, uint32_t ammo);
    void setActiveWeapon(uint32_t slot);
    void jump();
    btRigidBody* getPhysCharacter();
};
