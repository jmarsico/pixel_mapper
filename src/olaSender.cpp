//
//  olaSender.cpp
//  TOPAZ_oF
//
//  Created by Ultra Low Res Studuio on 12/14/17.
//
//

#include "olaSender.h"

void OlaSender::setup(int olaPort, int numUniverses){
    osc.setup("127.0.0.1", olaPort);
    for(int i = 0; i < numUniverses; i++){
        universe u;
        u.num = i+1;
        u.address = "/dmx/universe/";
        u.address += ofToString(i+1);
        u.buffer.resize(512, 0);
        universes.push_back(u);
    }

    ofLogNotice("ola sender") << "setup " << " universes: " << universes.size() << "olaPort: " << olaPort;

}


void OlaSender::setChannel(int _uni, int _chan, int val){

    //make sure the value is between 0 and 255
    val = ofClamp(val, 0, 255);
//    ofLog() << "val: " << val;

    //convert int to byte
    uint8_t valByte = val;

    //add the byte to the relevant universe and channel
    for(auto& u : universes){
        if(u.num == _uni){
            u.buffer[_chan-1] = valByte;
        }
    }

}

void OlaSender::sendToOla(){

    for(auto& u : universes){


        char blobBuff[512];
        memcpy(blobBuff, &u.buffer, sizeof(u.buffer));


        ofBuffer buff;
        buff.set(reinterpret_cast<const char*>(u.buffer.data()), u.buffer.size());

        //set up a message;
        ofxOscMessage m;
        m.setAddress(u.address);
        m.addBlobArg(buff);
        osc.sendMessage(m);
    }

}
