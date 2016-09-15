#include "Mover.h"

Mover::Mover (float m, float x, float y) {
        mass = m;
        G = 1;
        
        location = ofVec2f(x,y);
        velocity =  ofVec2f(0,0);
        acceleration = ofVec2f(0,0);
        
        maxspeed = 14;
        maxforce = 2;
    }

void Mover::applyForce (ofVec2f force) {
    ofVec2f f = force / mass;
    acceleration = acceleration + f;
}


void Mover::update(float _m, float _ms, float _mf) {
        mass = _m;
        maxspeed = _ms;
        maxforce = _mf;
        velocity += acceleration;
        velocity.limit(maxspeed);
        location += velocity;
        acceleration *= 0; // reset acceleration
}

void Mover::steer (ofVec2f target, float springiness) {
    ofVec2f force = target - location; // A vector pointing from the location to the target
    float dx = force.length(); // distance (length of vector from the location to the target)
    if (dx < springiness) { // 100 works
        float m = ofMap(dx, 0.0 , springiness, 0.0 , maxspeed);
        //force.setMag(m);
        force.normalize();
        force*=m;
    } else {
        //force.setMag(maxspeed);
        force.normalize();
        force*=maxspeed;
    }
    
    // Steering = distance minus Velocity
    ofVec2f steer = force - velocity;
    steer.limit(maxforce);  // Limit to maximum steering force
    applyForce(steer);
}

void Mover::display() {
    ofPushStyle();
        ofSetColor(0,255,0, 150);
        ofFill();		// draw "filled shapes"
        ofDrawCircle(location.x,location.y,mass*16);
    ofPopStyle();
}