#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

/*CEntity
    CLightEntity
        CBuilding
            CCompEntity
                CDynamicEntity
                    CAnimatedBuilding
                        CPhysical
                            CObject
                                CPed
                                    CVehicle
                                        CAutomobile
                                            CBike
                                                CBoat
                                                    CSubmarine
                                                        CTrain
                                                            CPtFxSortedEntity
                                                                CVehicleGlassComponentEntity
                                                                    CDummyObject*/

enum ObjectType
{
    Building,
    Ped,
    Vehicle
};

class CEntity
{
protected:
    glm::mat4 modelMatrix;
    glm::vec3 scale;
    float health = 100.0f;

private:
    bool visible;
    ObjectType type;

public:
    CEntity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_, ObjectType _type);
    ~CEntity();

    glm::vec3 position;
    glm::quat rotation;

    glm::vec3 getPosition() const
    {
        return position;
    }

    glm::mat4 getMatrix() const
    {
        return modelMatrix;
    }

    ObjectType getType() const
    {
        return type;
    }
};
