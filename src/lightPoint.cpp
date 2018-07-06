//
//  lightPoint.cpp
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#include "lightPoint.h"

void LightPoint::setup(ofVec2f _loc, int _universe, int _startChan){
    numSamples = 1;
    index = 0;
    loc = _loc;
    
    universe = _universe;
    startChan = _startChan;
    
    uint8_t start;
    start = 0;
    samples.push_back(start);
    currentCol.set(0,0,0);
}


void LightPoint::setAvgSamplingSize(int _numSamples){
    if(numSamples != _numSamples){
        numSamples = _numSamples;
        samples.clear();
        for(size_t i = 0; i < numSamples; i++){
            samples.push_back(0);
        }
        index = 0;
        ofLog() << "resetting sample size";
    }
    
    
    
}



void LightPoint::setCurrentCol(ofColor _col){
    index ++;
    if(index >= numSamples) index = 0;
    
    samples[index] = _col;
    
    currentCol = _col;
//    ofLog() << "set: " << currentVal;
    
}

ofColor LightPoint::getAvgCol(){
    int totalR = 0;
    int totalG = 0;
    int totalB = 0;
    
    for(size_t i = 0; i < numSamples; i++){
        
        totalR += samples[i].r;
        totalG += samples[i].g;
        totalB += samples[i].b;
    
    }
    
    int avR = totalR/numSamples;
    int avG = totalG/numSamples;
    int avB = totalB/numSamples;
    
    avgCol.set(avR, avG, avB);
    
    return avgCol;
}


void LightPoint::draw(){
    
//    ofLog() << "draw: " << currentVal;
    ofFill();
    ofSetColor(getAvgCol());
    ofDrawCircle(loc, 2.0);
   
}

void LightPoint::drawMap(){
    
    if(frbk == 1){
        ofSetColor(255,0,0);
    }
    else if(frbk == 2){
        ofSetColor(0,255,0);
    }
    ofFill();
    ofDrawCircle(loc, 2.0);
    
    string address = ofToString(universe);
    address += "|";
    address += ofToString(startChan);
    
//    ofDrawBitmapStringHighlight(address, loc.x - 2, loc.y +2);

}










