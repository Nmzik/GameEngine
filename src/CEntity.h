#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

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
    bool visible;
    ObjectType type;

protected:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::mat4 modelMatrix;
    float health = 100.0f;

public:
    CEntity(glm::vec3& pos, glm::quat rot, glm::vec3 scale_, ObjectType _type);
    ~CEntity();

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
