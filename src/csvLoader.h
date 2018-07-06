//
//  csvLoader.h
//  CLP_Knoxville_Wall
//
//  Created by Jakob Marsico on 4/8/16.
//
//

#ifndef __CLP_Knoxville_Wall__csvLoader__
#define __CLP_Knoxville_Wall__csvLoader__

#include <stdio.h>
#include "ofMain.h"
#include "ofxCsv.h"
#include "lightPoint.h"


class CsvLoader {
public:

    CsvLoader(){}
    void loadLightPoints(string filename);
    int getNumLights();
    ofVec2f getLightLoc(int index);

    string lightLocFileName;
    ofxCsv lightLocationCsv;

protected:
    vector<ofVec2f> lightPoints;

};

#endif /* defined(__CLP_Knoxville_Wall__csvLoader__) */
