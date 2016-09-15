#pragma once
#include "ofMain.h"

class Mover {
public: 
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    
    float mass;
    float G;
    
    float maxspeed;    // Maximum speed
    float maxforce;    // Maximum steering force
    
    float maxvelocity;
    float minvelocity;
    
    Mover( float m, float x, float y );
    
    void applyForce(ofVec2f force);
    
    void update(float _m, float _ms, float _mf);
    
    
    // A method that calculates a steering force towards a target
    void steer ( ofVec2f target, float springiness );
    
    void display();

};