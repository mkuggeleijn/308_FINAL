//
// Data structure for describing rivers to be rendered.
//
// by Adrian Cochrane

#pragma once

#include "cgra_math.hpp"

// 2D bounding box, viewed from above
struct Box {
    cgra::vec2 topleft;
    /*unsigned*/ float width, height;
};

struct river {
    float flow; // High fast the water should be moving
    float widthEnd; // How wide a river to render
    cgra::vec3 endPoint; // Where to render the river
    Box bbox; // To avoid generating sprites for offscreen rivers
    
    std::vector<river> upstream; // where the water may be flowing in from.
    // If no upstream, this represents a starting point for the river. 
};

// Quadtree for quickly finding only the rivers I need to generate.
// Operates in 2D coordinates as viewed from above
struct quadtree {
    cgra::vec2 center, size;
    quadtree *ne, *se, *sw, *nw;
    std::vector<river> rivers;
    
    quadtree(cgra::vec2 c, cgra::vec2 s) {
        center = c;
        size = s;
    }
    ~quadtree() {
        delete ne;
        delete se;
        delete sw;
        delete nw;
    }
    void lookup(Box bbox, std::vector<river>);
    void add(river);
};

// TODO figure out the area the index should cover
quadtree riverIndex(cgra::vec2(0.5, 0.5), cgra::vec2(1, 1));