#include "PhysicsConstants.hpp"
#include <cstdlib> // Include this for rand() function

const float GRAVITY = 0.98f;
const float TIMESTEP = 1; // seconds
const float ax = 0;
const float ay = 0;
const float vx = rand() % 1 - 1; // Random velocity between -10 and 10
const float vy = rand() % 1 - 1;
const float mass = 10.0f;
const float radius = 30;
const float omega = 0;
const float inertia = 0;
const float width = 50;
const float height = 50;
const float size = 500;
