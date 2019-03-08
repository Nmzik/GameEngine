#pragma once

#include <array>
#include <memory>
#include <vector>
#include "Entity.h"
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

class CPed : public Entity
{
    std::unique_ptr<btCapsuleShapeZ> physShape;
    std::unique_ptr<btRigidBody> body;

    std::array<CharacterWeaponSlot, 13> weapons;
    uint16_t currentWeapon = 0;
    bool inWater = false;
    glm::vec3 playerDirection;
    CVehicle* vehicle;
    YddLoader* player;
    bool on_ground = false;

    std::vector<YdrLoader*> playerModel;

public:
    CPed(glm::vec3 position, YddLoader* ydd);
    ~CPed();

    CPed& operator=(CPed&& other) = default;
    CPed(CPed&& other) = default;

    bool isAlive() const
    {
        return health > 0;
    }

    float getCurrentHealth() const
    {
        return health;
    }

    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition() const
    {
        return position;
    }
    glm::mat4& getMatrix();

    void takeDamage(float dmg);
    void physicsTick();
    void exitVehicle();
    void enterVehicle(CVehicle* nearestVehicle);
    CVehicle* getCurrentVehicle() const
    {
        return vehicle;
    }
    void addToInventory(uint32_t slot, uint32_t ammo);
    void setActiveWeapon(uint32_t slot);
    void jump();
    btRigidBody* getPhysCharacter();
};
