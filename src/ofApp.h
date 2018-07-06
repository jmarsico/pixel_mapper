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
    void drawContent();
    void saveToCsv();
    void loadLightPoints();

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
    ofParameter<bool> bPlay;
    ofParameter<int> sampleSize;
    ofParameter<bool> bLoadLightPoints;
    ofParameter<int> currLight;
    ofParameter<int> numRecorded;
    ofParameter<int> bgThresh;
    ofParameter<int> minSize;
    ofParameter<int> maxSize;
    ofParameter<int> dilation;
    ofParameter<int> sleep;
    ofParameter<bool> bResetThresh;
    ofParameter<string> FPS;
    ofParameter<bool> bSaveToCsv;
    ofParameter<float> contrast;
    
    
    int numLights;
    int currentLight;
    
    OlaSender olaSender;
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::RunningBackground background;
    ofImage thresholded;
    
    ofVideoGrabber grabber;
    
    std::vector<ofPoint> points;
    
    ofxCsv csv;
    CsvLoader csvLoader;
    
    vector<LightPoint> lightPoints;
    
    ofVideoPlayer player;
    string csvFileName;
    
    ofFbo samplerFbo;
    ofPixels samplerPix;
    
    ofShader shader;
    
		
};
