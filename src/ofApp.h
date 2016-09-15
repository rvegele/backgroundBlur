#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "Mover.h"
#include "Attractor.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofShader shaderBlurX;
        ofShader shaderBlurY;
        ofShader shaderCubic;
        ofShader shaderCubicDebug;
    
        ofFbo fboStaticBackground;
    
        ofFbo fboBlurOnePass;
    	ofFbo fboBlurTwoPass;
    	ofFbo fboCubic;
        ofFbo fboCubicDebug;
    
        ofImage imageBackground;
        ofImage imageTaskbar;
        ofImage imageDock;
    
        float blur;
    	float width;
        float height;
    
        bool debug;
        void initDebug();
    
        ofxPanel debugGui;
        ofParameter<float> blurX;
        ofParameter<float> blurY;
        ofParameter<float> spread;
        ofParameter<float> falloff;
        ofParameter<float> amount;
        //ofxColorSlider color;
        //ofxVec2Slider center;
        //ofxIntSlider circleResolution;
        ofxToggle hideImages;
        ofxToggle circular;
        ofParameter<float> circleRadius;
        ofParameter<float> innerCircleFade;
        ofParameter<float> outerCircleFade;
        //ofxButton twoCircles;
        //ofxButton ringButton;
        //ofxLabel screenSize;
        ofParameter<float> friction;
        ofParameter<float> moverMass;
        ofParameter<float> springiness;
        ofParameter<float> maxSpeed;
        ofParameter<float> maxForce;
    	ofParameter<float> positionOffset;
        ofxToggle motion;
        ofxToggle motionDebug;
        ofxToggle blurBg;
    
        void withImages();
        void withoutImages();
    
        // extra time
        Mover *mover;
        Mover *mover2;
        Attractor *attractor;
        float attractX;
    	float attractY;
        float repelX;
        float repelY;
    
        float dxPos2, dx2;
    
};
