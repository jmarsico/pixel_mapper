//
//  csvLoader.cpp
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#include "csvLoader.h"

void CsvLoader::loadLightPoints(string filename){
    
    //csv coordinates 3200w x 2400h
    lightLocationCsv.load(ofToDataPath(filename));
    ofLog() << "SIZE: " << ofToString(lightLocationCsv.getNumRows());
    for(int i = 0; i < lightLocationCsv.getNumRows(); i++){
//                ofLog() << "num: " << lightsCsv.data[i][0] << " x: " << lightsCsv.data[i][1] << " y: " << lightsCsv.data[i][2];
        
        ofVec2f lp;
        lp.set( lightLocationCsv.getRow(i).getInt(1),  -lightLocationCsv.getRow(i).getInt(2));
        lightPoints.push_back(lp);
    }
    ofLog() << "number of light points in lightPoints vector: " << lightPoints.size();
}

int CsvLoader::getNumLights(){
    return lightPoints.size();
}

ofVec2f CsvLoader::getLightLoc(int index){
    if(index < lightPoints.size()){
        return lightPoints[index];
    }
}
