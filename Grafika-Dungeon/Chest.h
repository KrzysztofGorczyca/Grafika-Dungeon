#pragma once
#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Chest {
public:
    glm::vec3 Position;
    glm::quat Rotation;
    float rotationRadians;

    bool spawned;

    // Constructor
    Chest(glm::vec3 position, float rotation)
        :Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , rotationRadians(rotation)
    {

    }
    inline glm::vec3 GetPosition() const { return Position; }
    inline float getRadians() const { return rotationRadians; }
};