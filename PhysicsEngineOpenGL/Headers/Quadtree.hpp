#ifndef Quadtree_hpp
#define Quadtree_hpp

#include "SFMLObjects.hpp"
#include "Object.hpp"
#include <vector>
#include <stdio.h>

class Quadtree {
public:
    Quadtree(int maxObjects, sf::FloatRect bounds);
    void insert(Object* object);
    std::vector<Object*> retrieve(sf::FloatRect range);
    void clear();

private:
    sf::FloatRect bounds;
    std::vector<Object*> objects;
    Quadtree* north_west;
    Quadtree* north_east;
    Quadtree* south_west;
    Quadtree* south_east;
    int maxObjects;

    void subdivide();
};

#endif /* Quadtree_hpp */
