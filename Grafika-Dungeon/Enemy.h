#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ModelPath "Assets/Skeleton/skeleton.obj"

class Enemy {
public:
    glm::vec3 Position;
    glm::quat Rotation;
    float Speed;
    float Health;
    float Damage;
    // Constructor
    Enemy(glm::vec3 position)
        :Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Health(100.0f)
        , Speed(1.0f)
		, Damage(25.0f)
    {

    }
    bool canDamage = true;

    // Destructor
    ~Enemy() {}

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        if(!isDead())
	    {
		    // Oblicz kierunek ruchu ignoruj¹c ró¿nicê w osi Y
	    	glm::vec3 direction = camera.getCameraPosition() - Position;
	    	direction.y = 0.0f;  // Ignoruj ró¿nicê w osi Y
	    	direction = glm::normalize(direction);  // Normalizuj wektor kierunku
	    	if(isDead())
	    	{
                canDamage = false;
	    		std::cout << "Enemy is dead" << std::endl;
	    	}

	    	// SprawdŸ odleg³oœæ i zaktualizuj pozycjê tylko w p³aszczyŸnie XZ
	    	float distance = glm::distance(glm::vec2(Position.x, Position.z), glm::vec2(camera.getCameraPosition().x, camera.getCameraPosition().z));
	    	if (distance < 5.0f && distance > 1.0f) {
	    		Position += direction * Speed * deltaTime;
	    		rotateEnemy(direction);
	    	}else if(distance < 1.0f){
	    		rotateEnemy(direction);
	    	}

	    	// Ustaw wspó³rzêdn¹ Y na sta³¹ wartoœæ (np. 0)
	    	Position.y = 0.0f;
	    }
    }

    void modifyHealth(float amount)
    {
    	Health += amount;
		if (Health < 0.0f) Health = 0.0f;
		if (Health > 100.0f) Health = 100.0f;
	}

    bool isDead()
    {
	    		if(Health <= 0.0f)
	    		{
                    Position = glm::vec3(0.0f, 0.0f, 100.0f);
	    			return true;
	    		}
                return false;
	}

    // Getters and setters
    inline glm::vec3 GetPosition() const{ return Position; }
    inline float GetHealth() const { return Health; }
    void UpdatePosition(glm::vec3 position)
    {
    	Position = position;
	}


private:
    void rotateEnemy(glm::vec3 direction) {
        // Oblicz k¹t obrotu na podstawie wektora kierunku
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);  // Zak³adamy, ¿e pocz¹tkowy kierunek przeciwnika to oœ Z
        glm::vec2 dir2D = glm::normalize(glm::vec2(direction.x, direction.z));
        glm::vec2 forward2D = glm::normalize(glm::vec2(forward.x, forward.z));
        float angle = atan2(dir2D.y, dir2D.x) - atan2(forward2D.y, forward2D.x);

        // Utwórz kwaternion obrotu wokó³ osi Y
        glm::quat rotationQuat = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Ustaw obrót przeciwnika
        Rotation = rotationQuat;
    }
};

#endif // ENEMY_H