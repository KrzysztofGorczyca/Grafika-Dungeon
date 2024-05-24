#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h" // Include necessary libraries
#include "Camera.h" // Include the Camera class


#define ModelPath "Assets/Enemy/Enemy.obj"

class Enemy {
public:
    glm::vec3 Position;
    float Speed;
    float Health;
    // Constructor
    Enemy(glm::vec3 position, float speed, float health)
        :Position(position), Speed(speed), Health(health)
    {

    }

    // Destructor
    ~Enemy() {}

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        // Example: Move towards the player
        glm::vec3 direction = glm::normalize(camera.getCameraPosition() - Position);
        if (glm::distance(Position, camera.getCameraPosition()) > 7.0f)
            Position += direction * Speed * deltaTime;
    }

    // Getters and setters
    glm::vec3 GetPosition() const { return Position; }
    float GetHealth() const { return Health; }

private:
    
};

#endif // ENEMY_H