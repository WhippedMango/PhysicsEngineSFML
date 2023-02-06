#ifndef Quadtree_hpp
#define Quadtree_hpp

#include "SFMLObjects.hpp"
#include "Object.hpp"
#include <vector>
#include <stdio.h>

class Quadtree {
public:
    // Bounding box representing the region of the quadtree
    sf::FloatRect bounds;
    // List of objects within the region
    std::vector<Object*> objects;
    // Child quadtrees representing subregions
    Quadtree* north_west;
    Quadtree* north_east;
    Quadtree* south_west;
    Quadtree* south_east;
    // Maximum number of objects before subdividing
    int maxObjects;
    
    Quadtree(int maxObjects, sf::FloatRect bounds) : maxObjects(maxObjects), bounds(bounds) {
        north_west = nullptr;
        north_east = nullptr;
        south_west = nullptr;
        south_east = nullptr;
    }
    
    // Insert an object into the quadtree
    void insert(Object* object) {
        // Check if the quadtree has reached its capacity
        if (objects.size() < maxObjects) {
            objects.push_back(object);
            return;
        }
        
        // Subdivide the quadtree if it has not yet been divided
        if (north_west == nullptr) {
            subdivide();
        }
        
        // Determine in which child quadtree the object belongs
        if (north_west->bounds.contains(object->x, object->y)) {
            north_west->insert(object);
        }
        else if (north_east->bounds.contains(object->x, object->y)) {
            north_east->insert(object);
        }
        else if (south_west->bounds.contains(object->x, object->y)) {
            south_west->insert(object);
        }
        else if (south_east->bounds.contains(object->x, object->y)) {
            south_east->insert(object);
        }
    }
    
    // Subdivide the quadtree
    void subdivide() {
        float x = bounds.left;
        float y = bounds.top;
        float w = bounds.width / 2.0f;
        float h = bounds.height / 2.0f;
        
        north_west = new Quadtree(maxObjects, sf::FloatRect(x, y, w, h));
        north_east = new Quadtree(maxObjects, sf::FloatRect(x + w, y, w, h));
        south_west = new Quadtree(maxObjects, sf::FloatRect(x, y + h, w, h));
        south_east = new Quadtree(maxObjects, sf::FloatRect(x + w, y + h, w, h));
    }
    
    // Retrieve all objects within a given bounding box
    std::vector<Object*> retrieve(sf::FloatRect range) {
        std::vector<Object*> foundObjects;
        // Check if the quadtree has been divided
        if (north_west == nullptr) {
            for (Object* object : objects) {
                if (range.contains(object->x, object->y)) {
                    foundObjects.push_back(object);
                }
            }
            return foundObjects;
        }
        // Check which child quadtrees the range intersects with
        if (north_west->bounds.intersects(range)) {
            std::vector<Object*> childObjects = north_west->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (north_east->bounds.intersects(range)) {
            std::vector<Object*> childObjects = north_east->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (south_west->bounds.intersects(range)) {
            std::vector<Object*> childObjects = south_west->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (south_east->bounds.intersects(range)) {
            std::vector<Object*> childObjects = south_east->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        return foundObjects;
    }
    
    // Clear the quadtree
    void clear() {
        objects.clear();
        if (north_west != nullptr) {
            north_west->clear();
            north_east->clear();
            south_west->clear();
            south_east->clear();
            delete north_west;
            delete north_east;
            delete south_west;
            delete south_east;
            north_west = nullptr;
            north_east = nullptr;
            south_west = nullptr;
            south_east = nullptr;
        }
    }
};


#endif /* Quadtree_hpp */
