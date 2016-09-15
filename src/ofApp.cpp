#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
//#ifdef TARGET_OPENGLES
//    shaderBlurX.load("shadersES2/shaderBlurX");
//    shaderBlurY.load("shadersES2/shaderBlurY");
//#else
//    if(ofIsGLProgrammableRenderer()){
//        shaderBlurX.load("shadersGL3/shaderBlurX");
//        shaderBlurY.load("shadersGL3/shaderBlurY");
//    }else{
        shaderBlurX.load("shadersGL2/shaderBlurX");
        shaderBlurY.load("shadersGL2/shaderBlurY");
    	shaderCubic.load("cubic");
        shaderCubicDebug.load("cubicDebug");
//    }
//#endif
    
    imageBackground.load("img/background_static2000.png");
    imageTaskbar.load("img/taskbar.png");
    imageDock.load("img/dock.png");
    
    fboStaticBackground.allocate( imageBackground.getWidth(), imageBackground.getHeight());
    fboBlurOnePass.allocate(imageBackground.getWidth(), imageBackground.getHeight());
    fboBlurTwoPass.allocate(imageBackground.getWidth(), imageBackground.getHeight());
    fboCubic.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    fboCubicDebug.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    
    width = 800.00;
    height = 800.00;
    
    debug = true;
    initDebug();
    
    fboStaticBackground.begin();
    ofClear(0,0,0, 255);
    fboStaticBackground.end();
    
    fboBlurOnePass.begin();
    ofClear(0,0,0,255);
    fboBlurOnePass.end();
    
    fboBlurTwoPass.begin();
    ofClear(0,0,0,255);
    fboBlurTwoPass.end();
    
    fboCubic.begin();
    ofClear(0,0,0,255);
    fboCubic.end();
    
    fboCubicDebug.begin();
    ofClear(0,0,0,255);
    fboCubicDebug.end();
    
    
    // extra stuff
    attractor = new Attractor(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    mover     = new Mover(2.00, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    mover2    = new Mover(2.00, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    
    attractX = ofGetWindowWidth()/4*3;
    attractY = ofGetWindowHeight()/2;
    
    repelX = ofGetWindowWidth()/4;
    repelY = ofGetWindowHeight()/2;
    
    dx2 = 0.00;
    dxPos2 = 0.00;
}

//--------------------------------------------------------------
void ofApp::update(){
    //cout << "mx: " << mouseX << " | my : " << mouseY << "\n";
    attractor -> update(ofGetMouseX(), ofGetMouseY());
    
    //EXTRA BEGIN
    ofVec2f attractorLocation;
    attractorLocation.set(attractor->location.x, attractor->location.y);
    float sprng = springiness;
    
    mover->steer(attractorLocation, sprng);
    mover2->steer(mover->location, sprng);
    
    // A vector pointing from the location to the target
    float distance = attractor->location.distance(mover->location);
    float distanceMM = mover->location.distance(mover2->location);
    
    // constrain the distance to not more than 800 px
    distance = ofClamp(distance, 0, 800);
    distanceMM = ofClamp(distanceMM, 0, 800);
    
    int vel_abs = (int)ofMap(distance, 0, 800, 15, 125); // 30 - 100
    int vel_absMM = (int)ofMap(distanceMM, 0, 800, 15, 125); // 30 - 100
    
    //println(vel_abs);
    //myPort.write(vel_abs);
    
    ofVec2f mover_velocity(mover->velocity);
    ofVec2f mover_velocity2(mover2->velocity);
    
    // c - friction value
    float c = friction;
    
    mover_velocity *= -1;
    mover_velocity.normalize();
    mover_velocity *=c;
    mover->applyForce(mover_velocity);
    
    mover_velocity2*=-1;
    mover_velocity2.normalize();
    mover_velocity2*=c;
    mover2->applyForce(mover_velocity2);
    
    // RUBBER BAND BEGIN
    float dx = mover->location.distance(attractor->location);
    dx2 = mover2->location.distance(mover->location);
    
    float dxPos = dx;
    dxPos2 = dx2;
    
    if ( mover->location.x > attractor->location.x ) { dxPos = -dx; } else { dxPos = dx; }  // cia reikia padaryti gradual perejima (fade)
    if ( mover2->location.x > mover->location.x ) { dxPos2 = -dx2; } else { dxPos2 = dx2; } // cia reikia padaryti gradual perejima (fade)
    
    // for background moving size
    
    dxPos = ofClamp(dxPos, -450, 450);
    dxPos2 = ofClamp(dxPos2, -450, 450);
    dx = ofClamp(dx, 0, 450);
    dx2 = ofClamp(dx2, 0, 450);
    int linija = (int)dx;
    
 
     dxPos = ofMap(dxPos, -450, 450, positionOffset, -positionOffset);
     dx = ofMap(dx, 0, 450, 0, 20);
     dxPos2 = ofMap(dxPos2, -450, 450, positionOffset, -positionOffset);
     dx2 = ofMap(dx2, 0, 450, 0, 20);
    

    
    // RUBBER BAND END
    float _m = moverMass;
    float _ms = maxSpeed;
    float _mf = maxForce;
    
    mover->update(_m, _ms, _mf);
    mover2->update(_m, _ms, _mf);
    //EXTRA END
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //blurPosition = ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    
    
    if(hideImages) {
        withoutImages();
    } else {
        withImages();
    }
    
    if( motionDebug ) {
        attractor->display(); // basically shows mouse position
        mover->display();
        mover2->display();
        ofPushStyle();
        ofSetColor(255,255,255, 150);
        ofFill();		// draw "filled shapes"
        ofDrawCircle(ofGetWindowWidth()/2+ dxPos2, ofGetWindowHeight()/2, 10+dx2, 10+dx2);
        ofPopStyle();
    }
    
    
    if (debug) {
        debugGui.draw();
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(ofGetWindowWidth() - 70, 20));
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout << blur << "\n";

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;

        case 'g':
            debug = !debug;
            break;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    fboCubic.allocate(width, height);
    fboCubicDebug.allocate(width, height);
    //cout << "width: " << width << " | height: " << height << "\n";
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::withImages(){

    // -----
    fboStaticBackground.begin();
    ofClear(0, 0, 0, 0);
        ofPushMatrix();
        //ofScale(0.5, 0.5);
        if (motion) { ofTranslate(dxPos2, 0.0); }
        imageBackground.draw(-280,0);
        ofPopMatrix();
    fboStaticBackground.end();
    
    // -----
    fboBlurOnePass.begin();
    ofClear(0, 0, 0, 0);
    shaderBlurX.begin();
    shaderBlurX.setUniform1f("blurAmnt", blurX);
    imageBackground.draw(0,0);
    shaderBlurX.end();
    fboBlurOnePass.end();
    
    // -----
    fboBlurTwoPass.begin();
    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    //ofScale(0.5, 0.5);
    shaderBlurY.begin();
    shaderBlurY.setUniform1f("blurAmnt", blurX*0.75);
        ofPushMatrix();
        if (motion) { ofTranslate(dxPos2, 0.0); }
        fboBlurOnePass.draw(-280, 0);
        ofPopMatrix();
    shaderBlurY.end();
    ofPopMatrix();
    fboBlurTwoPass.end();
    
    // -----
    fboCubic.begin();
    ofClear(0, 0, 0, 0);
    shaderCubic.begin();
    shaderCubic.setUniformTexture("tex0", fboBlurTwoPass, 1);
    shaderCubic.setUniform2f("screenSize", width, height);
    float mPosX = ofMap(ofGetMouseX(), 0, 1440, 0, 1);
    float mPosY = ofMap(ofGetMouseY(), 0, 900, 0, 1);
    shaderCubic.setUniform2f("mousePos", mPosX, mPosY);
    shaderCubic.setUniform1f("spread", spread);
    shaderCubic.setUniform1f("falloff", falloff);
    shaderCubic.setUniform1f("amount", amount);
    
    // circle
    if ( circular ) {
        shaderCubic.setUniform1i("piesiamApskritima", 1);
    } else {
        shaderCubic.setUniform1i("piesiamApskritima", 0);
    }
    
    shaderCubic.setUniform1f("circleRadius", circleRadius);
    shaderCubic.setUniform1f("innerCircleFade", innerCircleFade);
    shaderCubic.setUniform1f("outerCircleFade", outerCircleFade);
    fboCubic.draw(0, 0);
    shaderCubic.end();
    fboCubic.end();
    
    
    ofSetColor(ofColor::white);
    
    // -----
    //ofPushMatrix() ;
    //ofScale(0.5, 0.5);
    
    
    fboStaticBackground.draw(0, 0);
    //fboBlurTwoPass.draw(0, 0);
    //ofPopMatrix();
    if (blurBg) {
        fboCubic.draw(0, 0);
    }
    
    ofPushMatrix() ;
    ofScale(0.5, 0.5);
    imageTaskbar.draw(0,0);
    int dockX_pos = (ofGetWindowWidth() - (imageDock.getWidth()/2.0));
    int dockY_pos = (ofGetWindowHeight()*2) - imageDock.getHeight();//(ofGetWindowHeight() - imageDock.getHeight())/2.0;
    imageDock.draw(dockX_pos, dockY_pos);
    ofPopMatrix();
    
}

void ofApp::withoutImages() {
    // -----
    fboCubicDebug.begin();
    ofClear(0, 0, 0, 0);
    shaderCubicDebug.begin();
    //shaderCubicDebug.setUniformTexture("tex0", fboStaticBackground, 1);
    shaderCubicDebug.setUniform2f("screenSize", width, height);
    float mPosX = ofMap(ofGetMouseX(), 0, 1440, 0, 1);
    float mPosY = ofMap(ofGetMouseY(), 0, 900, 0, 1);
    shaderCubicDebug.setUniform2f("mousePos", mPosX, mPosY);
    shaderCubicDebug.setUniform1f("spread", spread);
    shaderCubicDebug.setUniform1f("falloff", falloff);
    shaderCubicDebug.setUniform1f("amount", amount);
    // circle
    if ( circular ) {
        shaderCubicDebug.setUniform1i("piesiamApskritima", 1);
    } else {
        shaderCubicDebug.setUniform1i("piesiamApskritima", 0);
    }
    
    shaderCubicDebug.setUniform1f("circleRadius", circleRadius);
    shaderCubicDebug.setUniform1f("innerCircleFade", innerCircleFade);
    shaderCubicDebug.setUniform1f("outerCircleFade", outerCircleFade);
    
    fboCubicDebug.draw(0, 0);
    shaderCubicDebug.end();
    fboCubicDebug.end();

    ofSetColor(ofColor::white);
    fboCubicDebug.draw(0, 0);
}

void ofApp::initDebug() {
    
    debugGui.setup();
    
    blurX = 3.700f;
    debugGui.add(blurX.set("blur", blurX, 0.000f, 10.000f));
    
    //blurY = 2.860f;
    //debugGui.add(blurY.set("blurY", blurY, 0.000f, 10.000f));
    
    spread = 1.50f;
    debugGui.add(spread.set("spread", spread, 0.000f, 4.000f));
    
    falloff = 0.000f;
    debugGui.add(falloff.set("sharpness", falloff, 0.000f, 0.500f));
    
    amount = 1.000f;
    debugGui.add(amount.set("clarity", amount, 1.000f, 0.000));
    
    debugGui.add(hideImages.setup("hideImages", false));
    debugGui.add(circular.setup("circular", false));
    
    circleRadius = 35.5f;
    debugGui.add(circleRadius.set("circleRadius", circleRadius, 0.000f, 200.000));
    innerCircleFade = 1.00f;
    debugGui.add(innerCircleFade.set("innerCircleFade", innerCircleFade, 0.000f, 1.000));
    outerCircleFade = 1.00f;
    debugGui.add(outerCircleFade.set("outerCircleFade", outerCircleFade, 0.000f, 1.000));
    
    debugGui.add(blurBg.setup("blurBg", true));
    
    friction = 0.10f;
    debugGui.add(friction.set("friction", friction, 0.000f, 0.500));
    
    moverMass = 2.00f;
    debugGui.add(moverMass.set("moverMass", moverMass, 1.000f, 10.000));
 
    springiness = 100.00f;
    debugGui.add(springiness.set("springiness", springiness, 15.000f, 500.000));
    
    maxSpeed = 14.00f;
    debugGui.add(maxSpeed.set("maxSpeed", maxSpeed, 0.000f, 50.000));
   
    maxForce = 2.00f;
    debugGui.add(maxForce.set("maxForce", maxForce, 0.000f, 50.000));
    
    positionOffset = 10.00;
    debugGui.add(positionOffset.set("positionOffset", positionOffset, 0.000f, 150.000));
    
    debugGui.add(motion.setup("motion", false));
    debugGui.add(motionDebug.setup("motionDebug", false));
}