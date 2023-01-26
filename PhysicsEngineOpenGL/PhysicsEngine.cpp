//
//  PhysicsEngine.cpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 19/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

#include "PhysicsEngine.hpp"
#include "PhysicsConstants.hpp"

#include <math.h>

PhysicsEngine::PhysicsEngine() {
    // constructor code
}

PhysicsEngine::~PhysicsEngine() {
    // destructor code
}

void PhysicsEngine::applyGravity(Object &object) {
    object.ay = GRAVITY;
}

void PhysicsEngine::updatePosition(Object &object, float timestep) {
    object.x += object.vx * timestep + 0.5 * object.ax * timestep * timestep;
    object.y += object.vy * timestep + 0.5 * object.ay * timestep * timestep;
}

void PhysicsEngine::updateVelocity(Object &object, float timestep) {
    object.vx += object.ax * timestep;
    object.vy += object.ay * timestep;
}

bool PhysicsEngine::checkCollision(Object &object1, Object &object2) {
    if (object1.shape == "circle" && object2.shape == "circle") {
        // Get the distance between the centers of the circles
        float distx = object1.x - object2.x;
        float disty = object1.y - object2.y;
        float distance = sqrt(distx*distx + disty*disty);

        // Check if the distance between the centers is less than the sum of the radii
        if (distance < object1.radius + object2.radius) {
            return true;
        }
    } else if (object1.shape == "square" && object2.shape == "square") {
        // Get the distance between the centers of the squares
        float distx = object1.x - object2.x;
        float disty = object1.y - object2.y;

        // Check if the distance between the centers is less than the sum of half the side lengths
        if (abs(distx) < (object1.size + object2.size) / 2 && abs(disty) < (object1.size + object2.size) / 2) {
            return true;
        }
    } else if (object1.shape == "circle" && object2.shape == "square") {
        // Get the distance between the center of the circle and the closest point on the square
        float distx = abs(object1.x - object2.x);
        float disty = abs(object1.y - object2.y);

        if (distx > (object2.size / 2 + object1.radius)) { return false; }
        if (disty > (object2.size / 2 + object1.radius)) { return false; }

        if (distx <= (object2.size / 2)) { return true; }
        if (disty <= (object2.size / 2)) { return true; }

        float cornerDistance_sq = pow((distx - object2.size / 2), 2) + pow((disty - object2.size / 2), 2);

        return (cornerDistance_sq <= pow(object1.radius, 2));
    } else if (object1.shape == "square" && object2.shape == "circle") {
        // Use the previous branch with the object positions swapped
        return checkCollision(object2, object1);
    }
    return false;
}

void PhysicsEngine::handleCollision(Object &object1, Object &object2, float COR) {
    // Get the normal vector between the objects
    float normx = object2.x  - object1.x;
    float normy = object2.y - object1.y;
    float new_mag = sqrt(normx*normx + normy*normy);
    float damp = 0.8;
    normx /= new_mag;
    normy /= new_mag;
    
    // Get the relative normal and tangent velocities
    float relNormalVel = (object1.vx - object2.vx) * normx + (object1.vy - object2.vy) * normy + (object1.omega - object2.omega) * object1.inertia * normx;
    float relTangentVel = (object1.vx - object2.vx) * -normy + (object1.vy - object2.vy) * normx + (object1.omega - object2.omega) * object1.inertia * -normy;
    
    if (relNormalVel > 0) {
        
        // Project the velocities onto the normal and tangent vectors
        float dot1 = object1.vx*normx + object1.vy*normy + object1.omega * object1.inertia * normx;
        float dot2 = object2.vx*normx + object2.vy*normy + object2.omega * object2.inertia * normx;
        float dot3 = relTangentVel * damp;
        
        // Calculate the new velocities and angular velocities using conservation of momentum
        float mass1 = object1.mass;
        float mass2 = object2.mass;
        float total_mass = mass1 + mass2;
        
        // Dot product with COR
        object1.vx = (dot1*(mass1 - mass2*COR) + 2*mass2*dot2*COR)*normx/total_mass + dot3*-normy;
        object1.vy = (dot1*(mass1 - mass2*COR) + 2*mass2*dot2*COR)*normy/total_mass + dot3*normx;
        object1.omega = (dot1*(object1.inertia - object2.inertia*COR) + 2*object2.inertia*dot2*COR)*normx/total_mass + dot3*-normy;
        
        object2.vx = (dot2*(mass2 - mass1*COR) + 2*mass1*dot1*COR)*normx/total_mass + dot3*-normy;
        object2.vy = (dot2*(mass2 - mass1*COR) + 2*mass1*dot1*COR)*normx/total_mass + dot3*normx;
        object2.omega = (dot2*(object2.inertia - object1.inertia*COR) + 2*object1.inertia*dot2*COR)*normx/total_mass + dot3*-normy;
    }
}

//bool PhysicsEngine::checkCollision(Object &object1, Object &object2) {
//    // Get the distance between the centers of the objects
//    float distx = object1.x - object2.x;
//    float disty = object1.y - object2.y;
//    float distance = sqrt(distx*distx + disty*disty);
//
//    // Check if the distance between the centers is less than the sum of the radii
//    if (distance < object1.radius + object2.radius) {
//        return true;
//    }
//    return false;
//}

//// Separating Axis Theorem (SAT) Collision + Conservation of momentum
//void PhysicsEngine::handleCollision(Object &object1, Object &object2, float COR) {
//    // Get the normal vector between the objects
//    float normx = object2.x  - object1.x;
//    float normy = object2.y - object1.y;
//    float new_mag = sqrt(normx*normx + normy*normy);
//    float damp = 0.8;
//    normx /= new_mag;
//    normy /= new_mag;
//
//    float relNormalVel = (object1.vx - object2.vx) * normx + (object1.vy - object2.vy) * normy;
//
//    // Get the tangent vector
//    float tanx = -normy;
//    float tany = normx;
//
//    if (relNormalVel > 0) {
//
//        // Project the velocities onto the normal and tangent vectors
//        float dot1 = object1.vx*normx + object1.vy*normy;
//        float dot2 = object2.vx*normx + object2.vy*normy;
//        float dot3 = (object1.vx*tanx + object1.vy*tany) * damp;
//        float dot4 = (object2.vx*tanx + object2.vy*tany) * damp;
//
//        // Calculate the new velocities using conservation of momentum
//        float mass1 = object1.mass;
//        float mass2 = object2.mass;
//        float total_mass = mass1 + mass2;
//
//        // Dot product with COR
//        object1.vx = (dot1*(mass1 - mass2*COR) + 2*mass2*dot2*COR)*normx/total_mass + dot3*tanx;
//        object1.vy = (dot1*(mass1 - mass2*COR) + 2*mass2*dot2*COR)*normy/total_mass + dot3*tany;
//        object2.vx = (dot2*(mass2 - mass1*COR) + 2*mass1*dot1*COR)*normx/total_mass + dot4*tanx;
//        object2.vy = (dot2*(mass2 - mass1*COR) + 2*mass1*dot1*COR)*normy/total_mass + dot4*tany;
//    }
//}

void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight, float COR) {
    if (object.x - object.radius < 0) {
        object.x = object.radius;
        object.vx = -object.vx;  // reverse the velocity in the x direction
    } else if (object.x + object.radius > windowWidth) {
        object.x = windowWidth - object.radius;
        object.vx = -object.vx;  // reverse the velocity in the x direction
    }
    if (object.y - object.radius < 0) {
        object.y = object.radius;
        object.vy = -object.vy;  // reverse the velocity in the y direction
    } else if (object.y + object.radius > windowHeight) {
        object.y = windowHeight - object.radius;
        object.vy = -object.vy;  // reverse the velocity in the y direction
    }
}

//void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight, float COR) {
//    // Get the normal vector between the object and the window edge
//    float normx = 0;
//    float normy = 0;
//    float damp = 0.8;
//    if (object.x - object.radius < 0) {
//        normx = 1;
//        object.x = object.radius;
//    } else if (object.x + object.radius > windowWidth) {
//        normx = -1;
//        object.x = windowWidth - object.radius;
//    }
//    if (object.y - object.radius < 0) {
//        normy = 1;
//        object.y = object.radius;
//    } else if (object.y + object.radius > windowHeight) {
//        normy = -1;
//        object.y = windowHeight - object.radius;
//    }
//
//    // Get the tangent vector
//    float tanx = -normy;
//    float tany = normx;
//
//    // Project the velocities onto the normal vector
//    float dot1 = object.vx*normx + object.vy*normy;
//
//    // Calculate the new velocity using conservation of momentum and COR
//    //    object.vx = (dot1 * (object.mass - 1*COR) + 2*1*dot1*COR)*normx/(object.mass + 1) + object.vx*tanx*damp;
//    //    object.vy = (dot1 * (object.mass - 1*COR) + 2*1*dot1*COR)*normy/(object.mass + 1) + object.vy*tany*damp;
//    object.vx = -dot1 * normx * COR + object.vx*tanx*damp;
//    object.vy = -dot1 * normy * COR + object.vy*tany*damp;
//}

//void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight) {
//    // Get the normal vector between the object and the window edge
//    float normx = 0;
//    float normy = 0;
//    float damp = 0.8;
//    if (object.x - object.radius < 0) {
//        normx = 1;
//        object.x = object.radius;
//    } else if (object.x + object.radius > windowWidth) {
//        normx = -1;
//        object.x = windowWidth - object.radius;
//    }
//    if (object.y - object.radius < 0) {
//        normy = 1;
//        object.y = object.radius;
//    } else if (object.y + object.radius > windowHeight) {
//        normy = -1;
//        object.y = windowHeight - object.radius;
//    }
//
//    // Get the tangent vector
//    float tanx = -normy;
//    float tany = normx;
//
//    // Project the velocities onto the normal and tangent vectors
//    float dot1 = object.vx*normx + object.vy*normy;
//    float dot3 = (object.vx*tanx + object.vy*tany) * damp;
//
//    // Update the velocity of the object
//    object.vx = -dot1 * normx * normx + dot3 * tanx;
//    object.vy = -dot1 * normy * normy + dot3 * tany;
//}


//void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight) {
//    // Get the normal vector between the object and the window edge
//    float normx = 0;
//    float normy = 0;
//    float damp = 0.8;
//    if (object.x - object.radius < 0) {
//        normx = 1;
//    } else if (object.x + object.radius/2 > windowWidth) {
//        normx = -1;
//    }
//    if (object.y - object.radius < 0) {
//        normy = 1;
//    } else if (object.y + object.radius > windowHeight) {
//        normy = -1;
//    }
//    float new_mag = sqrt(normx*normx + normy*normy);
//    normx /= new_mag;
//    normy /= new_mag;
//
//    // Get the tangent vector
//    float tanx = -normy;
//    float tany = normx;
//
//    // Project the velocities onto the normal and tangent vectors
//    float dot1 = object.vx*normx + object.vy*normy;
//    float dot3 = (object.vx*tanx + object.vy*tany) * damp;
//
//    // Update the position and velocity of the object
//    if (normx != 0) {
//        object.x = object.radius * normx;
//        object.vx = -dot1 * normx;
//    }
//    if (normy != 0) {
//        object.y = object.radius * normy;
//        object.vy = -dot1 * normy;
//    }
//    object.vx += dot3 * tanx;
//    object.vy += dot3 * tany;
//}

//void PhysicsEngine::handleCollisionWindow(Object &object, float windowWidth, float windowHeight) {
//    if (object.x - object.radius < 0) {
//        object.x = object.radius;
//        object.vx = -object.vx;  // reverse the velocity in the x direction
//    } else if (object.x + object.radius/2 > windowWidth) {
//        object.x = windowWidth - object.radius;
//        object.vx = -object.vx;  // reverse the velocity in the x direction
//    }
//    if (object.y - object.radius < 0) {
//        object.y = object.radius;
//        object.vy = -object.vy;  // reverse the velocity in the y direction
//    } else if (object.y + object.radius > windowHeight) {
//        object.y = windowHeight - object.radius;
//        object.vy = -object.vy;  // reverse the velocity in the y direction
//    }
//}


//bool PhysicsEngine::checkCollision(Object &object1, Object &object2) {
//    // Get the axis-aligned bounding boxes of the objects
//    sf::FloatRect aabb1 = object1.getAABB();
//    sf::FloatRect aabb2 = object2.getAABB();
//
//    // Check if the bounding boxes intersect
//    if (aabb1.intersects(aabb2)) {
//        // Get the normals of the objects
//        std::vector<sf::Vector2f> normals1 = object1.getNormals();
//        std::vector<sf::Vector2f> normals2 = object2.getNormals();
//
//        // Check for collision along each normal
//        for (auto normal : normals1) {
//            if (!checkCollisionAlongNormal(object1, object2, normal)) {
//                return false;
//            }
//        }
//        for (auto normal : normals2) {
//            if (!checkCollisionAlongNormal(object1, object2, normal)) {
//                return false;
//            }
//        }
//        return true;
//    }
//    return false;
//}


//// Separating Axis Theorem (SAT) Collision + Conservation of momentum
//void PhysicsEngine::handleCollision(Object &object1, Object &object2) {
//    // Get the normal vector between the objects
//    float normx = object2.x  - object1.x;
//    float normy = object2.y - object1.y;
//    float new_mag = sqrt(normx*normx + normy*normy);
//    normx /= new_mag;
//    normy /= new_mag;
//
//    // Get the tangent vector
//    float tx = -normy;
//    float ty = normx;
//
//    // Project the velocities onto the normal and tangent vectors
//    float dot1 = object1.vx*normx + object1.vy*normy;
//    float dot2 = object2.vx*normx + object2.vy*normy;
//    float dot3 = object1.vx*tx + object1.vy*ty;
//    float dot4 = object2.vx*tx + object2.vy*ty;
//
//    // Calculate the new velocities using conservation of momentum
//    float m1 = object1.mass;
//    float m2 = object2.mass;
//    float total_mass = m1 + m2;
//    object1.vx = (dot1*(m1 - m2) + 2*m2*dot2)*normx/total_mass + dot3*tx;
//    object1.vy = (dot1*(m1 - m2) + 2*m2*dot2)*normy/total_mass + dot3*ty;
//    object2.vx = (dot2*(m2 - m1) + 2*m1*dot1)*normx/total_mass + dot4*tx;
//    object2.vy = (dot2*(m2 - m1) + 2*m1*dot1)*normy/total_mass + dot4*ty;
//}

//void PhysicsEngine::handleCollision(Object &object1, Object &object2) {
//    // Calculate the distance between the centers of the circles
//    float dx = object1.x - object2.x;
//    float dy = object1.y - object2.y;
//    float distance = sqrt(dx*dx + dy*dy);
//
//    // Check if the circles are overlapping
//    if (distance < object1.radius + object2.radius) {
//        // Calculate the minimum translation distance (MTD) vector
//        float mtdx = (object1.radius + object2.radius - distance) * dx / distance;
//        float mtdy = (object1.radius + object2.radius - distance) * dy / distance;
//
//        // Reflect the velocities of the circles along the MTD vector
//        float total_mass = object1.mass + object2.mass;
//        float dvx = object2.vx - object1.vx;
//        float dvy = object2.vy - object1.vy;
//        float dot = dvx*mtdx + dvy*mtdy;
//        object1.vx += (2*object2.mass*dot/total_mass) * mtdx;
//        object1.vy += (2*object2.mass*dot/total_mass) * mtdy;
//        object2.vx -= (2*object1.mass*dot/total_mass) * mtdx;
//        object2.vy -= (2*object1.mass*dot/total_mass) * mtdy;
//    }
//}

// // Separating Axis Theorem (SAT) Collision
//void PhysicsEngine::handleCollision(Object &object1, Object &object2) {
//    // Get the normal vector between the objects
//    float newx = object2.x - object1.x;
//    float newy = object2.y - object1.y;
//    float new_mag = sqrt(newx*newx + newy*newy);
//    newx /= new_mag;
//    newy /= new_mag;
//
//    // Get the tangent vector
//    float tx = -newy;
//    float ty = newx;
//
//    // Project the velocities onto the normal and tangent vectors
//    float dot1 = object1.vx*newx + object1.vy*newy;
//    float dot2 = object2.vx*newx + object2.vy*newy;
//    float dot3 = object1.vx*tx + object1.vy*ty;
//    float dot4 = object2.vx*tx + object2.vy*ty;
//
//    // Update the velocities of the objects
//    object1.vx = (dot1*newx - dot2*newx) + dot3*tx;
//    object1.vy = (dot1*newy - dot2*newy) + dot3*ty;
//    object2.vx = (dot2*newx - dot1*newx) + dot4*tx;
//    object2.vy = (dot2*newy - dot1*newy) + dot4*ty;
//}
