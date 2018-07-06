#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxJSON.h"
#include "ofxCsv.h"
#include "olaSender.h"
#include "lightPoint.h"
#include "csvLoader.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void saveToCsv();

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
    
    ofxPanel gui;
    ofParameter<bool> bStartMapping;
    ofParameter<bool> bIsMapping;
    ofParameter<int> currLight;
    ofParameter<int> bgThresh;
    ofParameter<bool> bResetThresh;
    ofParameter<string> FPS;
    ofParameter<bool> bSaveToCsv;
    
    
    int numLights;
    int currentLight;
    
    OlaSender olaSender;
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::RunningBackground background;
    ofImage thresholded;
    
    ofVideoGrabber grabber;
    
    std::vector<ofPoint> points;
    
    ofxCsv csv;
		
};
