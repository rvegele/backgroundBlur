#include "Attractor.h"

Attractor::Attractor(int posX, int posY) {
        x = posX;
        y = posY;
        location = ofVec2f(x,y);
}

void Attractor::update (int mx, int my) {
    location.x = mx;
    location.y = my;
}

void Attractor::display() {
    ofPushStyle();
        ofSetColor(255,0,0, 150);
        ofFill();		// draw "filled shapes"
        ofDrawCircle(location.x,location.y,10);
    ofPopStyle();
    //ellipseMode(CENTER);
    //fill(255, 0, 0);
    //ellipse(location.x,location.y,10,10); // mass * 2
}
