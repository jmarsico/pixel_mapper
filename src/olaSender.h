//
//  olaSender.hpp
//  TOPAZ_oF
//
//  Created by Ultra Low Res Studuio on 12/14/17.
//
//

#ifndef olaSender_hpp
#define olaSender_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"


class OlaSender {
public:
    OlaSender(){};
    void setup(int olaPort, int numUniverses);
    void setChannel(int universe, int chanNum, int val);
    void sendToOla();

    
    
    ofxOscSender osc;
    
    struct universe {
        int num;
        string address;
        vector<uint8_t> buffer;
    };
    
    vector<universe> universes;
    
    
    
    
};

#endif /* olaSender_hpp */
