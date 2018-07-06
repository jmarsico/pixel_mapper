//
//  lightPoint.h
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#ifndef __CLP_Knoxville_Wall__lightPoint__
#define __CLP_Knoxville_Wall__lightPoint__

#include <stdio.h>
#include "ofMain.h"

class LightPoint {
    
public:
    LightPoint(){}
    
    void setup(ofVec2f _loc, int _universe, int _startChan, int _frbk);
    void setAvgSamplingSize(int _numSamples);
    void setCurrentCol(ofColor _col);
    void draw();
    void drawMap();
    
    ofColor getAvgCol();
    ofVec2f getLoc() { return loc; }
    int getUniverse() { return universe; }
    int getStartChan() { return startChan; }
    int getSideFrontBack() { return frbk; }

    

    
    
protected:
    
    int numSamples;
    vector<ofColor> samples;
    int index;
    
    
    ofVec2f loc;
    ofColor avgCol;
    ofColor currentCol;
    int universe;
    int startChan;
    int frbk;
    
};

#endif /* defined(__CLP_Knoxville_Wall__lightPoint__) */
