#pragma once
#include "ofMain.h"

class Attractor {

public:
    
    
    Attractor(int posX, int posY);
    
    void update (int mx, int my);
    void display();
    
    ofVec2f location;   // Location
    int x, y;

        //ellipseMode(CENTER);
        //fill(255, 0, 0);
        //ellipse(location.x,location.y,10,10); // mass * 2

    
};