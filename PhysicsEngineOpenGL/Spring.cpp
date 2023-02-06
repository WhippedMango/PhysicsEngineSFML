#include "Spring.hpp"
#include "Object.hpp"
#include <math.h>
#include <SFML/System/Vector2.hpp>

class Spring {
public:
    float springConstant;
    float restLength;
    Object* object1;
    Object* object2;
   
    Spring(Object* object1, Object* object2, float restLength, float springConstant) {
        this->object1 = object1;
        this->object2 = object2;
        this->restLength = restLength;
        this->springConstant = springConstant;
    }


    void update() {
        float distance = sqrt(pow(object1->position.x - object2->position.x, 2) + pow(object1->position.y - object2->position.y, 2));
        sf::Vector2f force = (object1->position - object2->position) * (distance - restLength) * springConstant;
        object1->applyForce(-force);
        object2->applyForce(force);
    }
};
