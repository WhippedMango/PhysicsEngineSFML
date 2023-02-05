//
//  Quadtree.hpp
//  PhysicsEngineOpenGL
//
//  Created by Karl Davies on 24/01/2023.
//  Copyright © 2023 Karl Davies. All rights reserved.
//

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
    Quadtree* northWest;
    Quadtree* northEast;
    Quadtree* southWest;
    Quadtree* southEast;
    // Maximum number of objects before subdividing
    int maxObjects;
    
    Quadtree(int maxObjects, sf::FloatRect bounds) : maxObjects(maxObjects), bounds(bounds) {
        northWest = nullptr;
        northEast = nullptr;
        southWest = nullptr;
        southEast = nullptr;
    }
    
    // Insert an object into the quadtree
    void insert(Object* object) {
        // Check if the quadtree has reached its capacity
        if (objects.size() < maxObjects) {
            objects.push_back(object);
            return;
        }
        
        // Subdivide the quadtree if it has not yet been divided
        if (northWest == nullptr) {
            subdivide();
        }
        
        // Determine in which child quadtree the object belongs
        if (northWest->bounds.contains(object->x, object->y)) {
            northWest->insert(object);
        }
        else if (northEast->bounds.contains(object->x, object->y)) {
            northEast->insert(object);
        }
        else if (southWest->bounds.contains(object->x, object->y)) {
            southWest->insert(object);
        }
        else if (southEast->bounds.contains(object->x, object->y)) {
            southEast->insert(object);
        }
    }
    
    // Subdivide the quadtree
    void subdivide() {
        float x = bounds.left;
        float y = bounds.top;
        float w = bounds.width / 2.0f;
        float h = bounds.height / 2.0f;
        
        northWest = new Quadtree(maxObjects, sf::FloatRect(x, y, w, h));
        northEast = new Quadtree(maxObjects, sf::FloatRect(x + w, y, w, h));
        southWest = new Quadtree(maxObjects, sf::FloatRect(x, y + h, w, h));
        southEast = new Quadtree(maxObjects, sf::FloatRect(x + w, y + h, w, h));
    }
    
    // Retrieve all objects within a given bounding box
    std::vector<Object*> retrieve(sf::FloatRect range) {
        std::vector<Object*> foundObjects;
        // Check if the quadtree has been divided
        if (northWest == nullptr) {
            for (Object* object : objects) {
                if (range.contains(object->x, object->y)) {
                    foundObjects.push_back(object);
                }
            }
            return foundObjects;
        }
        // Check which child quadtrees the range intersects with
        if (northWest->bounds.intersects(range)) {
            std::vector<Object*> childObjects = northWest->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (northEast->bounds.intersects(range)) {
            std::vector<Object*> childObjects = northEast->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (southWest->bounds.intersects(range)) {
            std::vector<Object*> childObjects = southWest->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        if (southEast->bounds.intersects(range)) {
            std::vector<Object*> childObjects = southEast->retrieve(range);
            foundObjects.insert(foundObjects.end(), childObjects.begin(), childObjects.end());
        }
        return foundObjects;
    }
    
    // Clear the quadtree
    void clear() {
        objects.clear();
        if (northWest != nullptr) {
            northWest->clear();
            northEast->clear();
            southWest->clear();
            southEast->clear();
            delete northWest;
            delete northEast;
            delete southWest;
            delete southEast;
            northWest = nullptr;
            northEast = nullptr;
            southWest = nullptr;
            southEast = nullptr;
        }
    }
};


#endif /* Quadtree_hpp */
